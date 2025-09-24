#include "MainWindowState.h"
#include "utils/TimeCalculator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QDate>
#include <QTime>
#include <QDateTime>

MainWindowState::MainWindowState(QObject* parent) : QObject(parent), timerStatus(MainWindowState::TimerStatus::Stopped), timerValue(0), workdayEndNotified(false)
{
    for (int day = 1; day <= 7; ++day) {
        workSecondsPerDay[day] = 9 * 3600;
        minBreakSecondsPerDay[day] = 45 * 60;
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindowState::updateValue);

    autosaveTimer = new QTimer(this);
    connect(autosaveTimer, &QTimer::timeout, this, [this]() {
        saveToFile("state.json");
    });
    autosaveTimer->start(60 * 1000);
    loadSettings("settings.json");
    loadFromFile("state.json");
    loadDailyDurations(); // Load daily durations
    pruneOldDurations(); // Remove old data
    updateFinishTime();
}

int MainWindowState::getTotalSeconds() const {
    int today = QDate::currentDate().dayOfWeek();
    return workSecondsPerDay.value(today, 9 * 3600);
}

void MainWindowState::setTimeStatus(MainWindowState::TimerStatus status) {
    if (status == MainWindowState::TimerStatus::Running) start();
    else if (status == MainWindowState::TimerStatus::Paused) pause();
    else if (status == MainWindowState::TimerStatus::Resumed) resume();
    else stop();
}

void MainWindowState::start() {
    if (timerStatus == MainWindowState::TimerStatus::Stopped) {
        elapsedBeforePause = 0;
        timerStatus = MainWindowState::TimerStatus::Running;
        timerEvents.clear();
        startTime = QTime::currentTime();
        timer->start(1000);
        logEvent(TimerEvent::Start);
        setTimerValue(0);
        saveToFile("state.json");
        emit timerStatusChanged(timerStatus);
        emit timerValueChanged(timerValue);
        emit finishTimeChanged(getStatus() == TimerEvent::Stop ? getFinishTime() : calculateFinishTime());
    }
}

void MainWindowState::pause() {
    if (timerStatus == MainWindowState::TimerStatus::Running || timerStatus == MainWindowState::TimerStatus::Resumed) {
        timerStatus = MainWindowState::TimerStatus::Paused;
        elapsedBeforePause += startTime.secsTo(QTime::currentTime());
        timer->stop();
        emit timerStatusChanged(timerStatus);
        logEvent(TimerEvent::Pause);
    }
}

void MainWindowState::resume() {
    if (timerStatus == MainWindowState::TimerStatus::Paused) {
        timerStatus = MainWindowState::TimerStatus::Running;
        startTime = QTime::currentTime();
        timer->start(1000);
        emit timerStatusChanged(timerStatus);
        logEvent(TimerEvent::Resume);
    }
}

void MainWindowState::stop() {
    if (timerStatus == MainWindowState::TimerStatus::Running || timerStatus == MainWindowState::TimerStatus::Resumed) {
        timerStatus = MainWindowState::TimerStatus::Stopped;
        timer->stop();
        emit timerStatusChanged(timerStatus);
        logEvent(TimerEvent::Stop);

        // Record the day's duration when stopping
        if (!timerEvents.isEmpty()) {
            QDateTime firstStart = TimeCalculator::findFirstStartTime(timerEvents);
            if (firstStart.isValid()) {
                // Calculate total work time for the day
                QDateTime currentTime = QDateTime::currentDateTime();
                int totalWorkSeconds = firstStart.secsTo(currentTime);

                // Record the duration for this day
                recordDayDuration(firstStart.date(), totalWorkSeconds);
            }
        }
    }
}

QTime MainWindowState::getStartTime() const {
    QDateTime startTime;
    for (const auto& e : timerEvents) {
        if (e.type == TimerEvent::Start) {
            startTime = e.timestamp;
            break;
        }
    }
    return startTime.time();
}

QTime MainWindowState::getFinishTime() const {
    QDateTime finishTime;
    for (const auto& e : timerEvents) {
        if (e.type == TimerEvent::Stop) {
            finishTime = e.timestamp;
            break;
        }
    }
    return finishTime.time();
}


MainWindowState::TimerStatus MainWindowState::getStatus() const {
    return timerStatus;
}

int MainWindowState::getValue() const {
    return timerValue;
}

void MainWindowState::updateValue() {
    int elapsed = elapsedBeforePause + startTime.secsTo(QTime::currentTime());
    setTimerValue(elapsed);

    // Check if workday has ended
    checkWorkdayEnd();
}

void MainWindowState::setTimerValue(int value) {
    if (timerValue != value) {
        timerValue = value;
        emit timerValueChanged(timerValue);
    }
}

void MainWindowState::logEvent(TimerEvent::EventType type) {
    TimerEvent event{ type, QDateTime::currentDateTime() };
    timerEvents.append(event);
}

bool MainWindowState::saveToFile(const QString& fileName) const {
    QJsonObject root;

    root["formatVersion"] = 1;
    root["timerValue"] = timerValue;
    root["elapsedBeforePause"] = elapsedBeforePause;
    root["status"] = static_cast<int>(timerStatus);
    root["lastSaved"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonArray eventsArray;
    for (const auto& e : timerEvents) {
        eventsArray.append(e.toJson());
    }
    root["events"] = eventsArray;

    QJsonDocument doc(root);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool MainWindowState::loadFromFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();

    int formatVersion = root["formatVersion"].toInt(1);
    Q_UNUSED(formatVersion);

    timerValue = root["timerValue"].toInt(0);
    elapsedBeforePause = root["elapsedBeforePause"].toInt(0);
    timerStatus = static_cast<MainWindowState::TimerStatus>(root["status"].toInt(MainWindowState::TimerStatus::Stopped));

    timerEvents.clear();
    QJsonArray eventsArray = root["events"].toArray();
    for (const auto& val : eventsArray) {
        timerEvents.append(TimerEvent::fromJson(val.toObject()));
    }

    QString lastSavedStr = root["lastSaved"].toString();
    if (!lastSavedStr.isEmpty() && (timerStatus == MainWindowState::TimerStatus::Running || timerStatus == MainWindowState::TimerStatus::Resumed)) {
        QDateTime lastSaved = QDateTime::fromString(lastSavedStr, Qt::ISODate);
        if (lastSaved.isValid()) {
            qint64 secsPassed = lastSaved.secsTo(QDateTime::currentDateTime());
            timerValue += static_cast<int>(secsPassed);
        }
    }

    elapsedBeforePause = timerValue;

    emit timerValueChanged(timerValue);
    emit timerStatusChanged(timerStatus);
    emit finishTimeChanged(getStatus() == TimerEvent::Stop ? getFinishTime() : calculateFinishTime());

    if (timerStatus == MainWindowState::TimerStatus::Running || timerStatus == MainWindowState::TimerStatus::Resumed) {
        startTime = QTime::currentTime();
        timer->start(1000);
    }

    return true;
}

void MainWindowState::loadSettings(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();

    for (int day = 1; day <= 7; ++day) {
        QString key = QString::number(day); // 1..7
        if (obj.contains(key)) {
            QJsonObject dayObj = obj[key].toObject();
            workSecondsPerDay[day] = dayObj.value("workSeconds").toInt(9*3600);
            minBreakSecondsPerDay[day] = dayObj.value("breakSeconds").toInt(3600);
        }
    }

    for (int day = 1; day <= 7; ++day) {
        qDebug() << "Day" << day
                 << "workSeconds:" << workSecondsPerDay[day]
                 << "breakSeconds:" << minBreakSecondsPerDay[day];
    }

}

QTime MainWindowState::calculateFinishTime() const {
    int today = QDate::currentDate().dayOfWeek();
    int requiredWork = getTotalSeconds();
    int minBreak = minBreakSecondsPerDay.value(today, 0);

    return TimeCalculator::calculateFinishTime(
        timerEvents,
        requiredWork,
        minBreak,
        QDateTime::currentDateTime()
    );
}

void MainWindowState::updateFinishTime() {
    emit finishTimeChanged(getStatus() == TimerEvent::Stop ? getFinishTime() : calculateFinishTime());
}

void MainWindowState::setWorkSecondsForDay(int day, int seconds) {
    workSecondsPerDay[day] = seconds;
    updateFinishTime();
}

void MainWindowState::setMinBreakSecondsForDay(int day, int seconds) {
    minBreakSecondsPerDay[day] = seconds;
    updateFinishTime();
}

void MainWindowState::recordDayDuration(const QDate& date, int seconds) {
    dailyWorkDurations[date] = seconds;
    saveDailyDurations();
}

void MainWindowState::pruneOldDurations() {
    QDate oneMonthAgo = QDate::currentDate().addMonths(-1);
    QMap<QDate, int> prunedDurations;

    for (auto it = dailyWorkDurations.begin(); it != dailyWorkDurations.end(); ++it) {
        if (it.key() >= oneMonthAgo) {
            prunedDurations[it.key()] = it.value();
        }
    }

    dailyWorkDurations = prunedDurations;
}

void MainWindowState::saveDailyDurations() const {
    QJsonObject root;

    // Add format version
    root["formatVersion"] = 1;

    // Add daily durations
    QJsonObject durationsObj;
    for (auto it = dailyWorkDurations.begin(); it != dailyWorkDurations.end(); ++it) {
        durationsObj[it.key().toString(Qt::ISODate)] = it.value();
    }
    root["dailyDurations"] = durationsObj;

    // Save to file
    QJsonDocument doc(root);
    QFile file("daily_durations.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void MainWindowState::loadDailyDurations() {
    QFile file("daily_durations.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();

    int formatVersion = root["formatVersion"].toInt(1);
    Q_UNUSED(formatVersion);

    QJsonObject durationsObj = root["dailyDurations"].toObject();
    for (auto it = durationsObj.begin(); it != durationsObj.end(); ++it) {
        QDate date = QDate::fromString(it.key(), Qt::ISODate);
        if (date.isValid()) {
            dailyWorkDurations[date] = it.value().toInt(0);
        }
    }
}

bool MainWindowState::removePauseResumePair(int pauseRow) {
    // Find the pause and resume events that correspond to the given row
    QDate today = QDate::currentDate();
    QVector<TimerEvent> todayEvents;
    QVector<int> todayEventIndices;

    // Filter events to only include those from the current day
    for (int i = 0; i < timerEvents.size(); ++i) {
        const auto& event = timerEvents[i];
        if (event.timestamp.date() == today) {
            todayEvents.append(event);
            todayEventIndices.append(i);
        }
    }

    // Find the pause/resume pair for the given row
    QDateTime pauseStart;
    int pauseIndex = -1;
    int resumeIndex = -1;
    int currentRow = 0;

    for (int i = 0; i < todayEvents.size(); ++i) {
        const auto& event = todayEvents[i];
        if (event.type == TimerEvent::Pause) {
            pauseStart = event.timestamp;
            pauseIndex = todayEventIndices[i];
        } else if (event.type == TimerEvent::Resume && pauseStart.isValid()) {
            if (currentRow == pauseRow) {
                resumeIndex = todayEventIndices[i];
                break;
            }
            pauseStart = QDateTime();
            currentRow++;
        }
    }

    // If we found both events, remove them
    if (pauseIndex != -1 && resumeIndex != -1) {
        // Remove the resume event first (higher index)
        if (resumeIndex > pauseIndex) {
            timerEvents.remove(resumeIndex);
            timerEvents.remove(pauseIndex);
        } else {
            timerEvents.remove(pauseIndex);
            timerEvents.remove(resumeIndex);
        }

        // Save to file and update
        saveToFile("state.json");
        emit timerStatusChanged(timerStatus);
        emit timerValueChanged(timerValue);
        emit finishTimeChanged(getStatus() == TimerEvent::Stop ? getFinishTime() : calculateFinishTime());

        return true;
    }

    return false;
}

void MainWindowState::updateStartTime(const QTime& newStartTime) {
    QTime oldStartTime = getStartTime();

    for (auto& event : timerEvents) {
        if (event.type == TimerEvent::Start) {
            QDateTime newDateTime = event.timestamp;
            newDateTime.setTime(newStartTime);
            event.timestamp = newDateTime;
            break;
        }
    }

    if (timerStatus == Running || timerStatus == Resumed) {
        int timeDiff = oldStartTime.secsTo(newStartTime);

        elapsedBeforePause -= timeDiff;

        updateValue();
    }

    emit finishTimeChanged(getStatus() == TimerEvent::Stop ? getFinishTime() : calculateFinishTime());

    saveToFile("state.json");
}

void MainWindowState::updateFinishTime(const QTime& newFinishTime) {
    QTime oldFinishTime = getFinishTime();

    for (auto& event : timerEvents) {
        if (event.type == TimerEvent::Stop) {
            QDateTime newDateTime = event.timestamp;
            newDateTime.setTime(newFinishTime);
            event.timestamp = newDateTime;
            break;
        }
    }

    if (timerStatus == Stopped) {
        int timeDiff = oldFinishTime.secsTo(newFinishTime);
        setTimerValue(timerValue + timeDiff);
    }

    emit finishTimeChanged(getStatus() == TimerEvent::Stop ? getFinishTime() : calculateFinishTime());
    saveToFile("state.json");
}

// Implementation of notification methods
void MainWindowState::checkWorkdayEnd() {
    // Only check if the timer is running and we haven't already notified
    if ((timerStatus == Running || timerStatus == Resumed) && !workdayEndNotified) {
        if (isWorkdayEnded()) {
            workdayEndNotified = true;
            emit workdayEnded();
        }
    }

    // Reset notification flag if workday is not ended (allows for notification on next end)
    if (!isWorkdayEnded()) {
        workdayEndNotified = false;
    }
}

// Remove const qualifier to fix compilation error
bool MainWindowState::isWorkdayEnded() {
    // Get current time
    QTime now = QTime::currentTime();

    // Calculate finish time
    QTime finishTime = calculateFinishTime();

    // Check if current time is past the finish time
    // We consider workday ended if we're within 1 minute of the finish time
    // or if we've passed it
    int secondsToFinish = now.secsTo(finishTime);
    return secondsToFinish <= 0;
}