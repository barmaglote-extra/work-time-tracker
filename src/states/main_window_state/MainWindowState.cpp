#include "MainWindowState.h"

MainWindowState::MainWindowState(QObject* parent) : QObject(parent), timerStatus(Stopped), timerValue(0)
{
    for (int day = 1; day <= 7; ++day) {
        workSecondsPerDay[day] = 9 * 3600;
        minBreakSecondsPerDay[day] = 3600;
    }

    loadSettings("settings.json");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindowState::updateValue);

    autosaveTimer = new QTimer(this);
    connect(autosaveTimer, &QTimer::timeout, this, [this]() {
        saveToFile("state.json");
    });
    autosaveTimer->start(60 * 1000);
}

int MainWindowState::getTotalSeconds() const {
    int today = QDate::currentDate().dayOfWeek();
    if (workSecondsPerDay.contains(today)) {
        return workSecondsPerDay[today];
    }
    return 9 * 10 * 10;
}

void MainWindowState::setTimeStatus(TimerStatus status) {
    if (status == Running) start();
    else if (status == Paused) pause();
    else if (status == Resumed) resume();
    else stop();
}

void MainWindowState::start() {
    if (timerStatus == Stopped) {
        elapsedBeforePause = 0;
        timerStatus = Running;
        timerEvents.clear();
        startTime = QTime::currentTime();
        timer->start(1000);
        logEvent(TimerEvent::Start);
        setTimerValue(0);
        saveToFile("state.json");
        emit timerStatusChanged(timerStatus);
    }
}

void MainWindowState::pause() {
    if (timerStatus == Running || timerStatus == Resumed) {
        timerStatus = Paused;
        elapsedBeforePause += startTime.secsTo(QTime::currentTime());
        timer->stop();
        emit timerStatusChanged(timerStatus);
        logEvent(TimerEvent::Pause);
    }
}

void MainWindowState::resume() {
    if (timerStatus == Paused) {
        timerStatus = Running;
        startTime = QTime::currentTime();
        timer->start(1000);
        emit timerStatusChanged(timerStatus);
        logEvent(TimerEvent::Resume);
    }
}

void MainWindowState::stop() {
    if (timerStatus == Running || timerStatus == Resumed) {
        timerStatus = Stopped;
        timer->stop();
        emit timerStatusChanged(timerStatus);
        logEvent(TimerEvent::Stop);
    }
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
    root["totalSeconds"] = totalSeconds;
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
    totalSeconds = root["totalSeconds"].toInt(0);
    elapsedBeforePause = root["elapsedBeforePause"].toInt(0);
    timerStatus = static_cast<TimerStatus>(root["status"].toInt(Stopped));

    timerEvents.clear();
    QJsonArray eventsArray = root["events"].toArray();
    for (const auto& val : eventsArray) {
        timerEvents.append(TimerEvent::fromJson(val.toObject()));
    }

    QString lastSavedStr = root["lastSaved"].toString();
    if (!lastSavedStr.isEmpty() && (timerStatus == Running || timerStatus == Resumed)) {
        QDateTime lastSaved = QDateTime::fromString(lastSavedStr, Qt::ISODate);
        if (lastSaved.isValid()) {
            qint64 secsPassed = lastSaved.secsTo(QDateTime::currentDateTime());
            timerValue += static_cast<int>(secsPassed);
        }
    }

    elapsedBeforePause = timerValue;

    emit timerValueChanged(timerValue);
    emit timerStatusChanged(timerStatus);

    if (timerStatus == Running || timerStatus == Resumed) {
        startTime = QTime::currentTime();
        timer->start(1000);
    }

    return true;
}

void MainWindowState::loadSettings(const QString& fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject obj = doc.object();

        QLocale locale;
        for (int day = 1; day <= 7; ++day) {
            QString dayName = locale.dayName(day, QLocale::LongFormat); // "Monday", ...
            if (obj.contains(dayName)) {
                QJsonObject dayObj = obj[dayName].toObject();
                workSecondsPerDay[day] = dayObj.value("workHours").toDouble(9.0) * 3600;
                minBreakSecondsPerDay[day] = dayObj.value("minBreakHours").toDouble(1.0) * 3600;
            }
        }
    }
}

QTime MainWindowState::calculateFinishTime() {
    if (timerEvents.isEmpty()) {
        return QTime();
    }

    int today = QDate::currentDate().dayOfWeek();
    int requiredWork = getTotalSeconds();
    int minBreak = minBreakSecondsPerDay.value(today, 0);

    QDateTime firstStart;
    for (const auto& ev : timerEvents) {
        if (ev.type == TimerEvent::Start) {
            firstStart = ev.timestamp;
            break;
        }
    }
    if (!firstStart.isValid()) {
        return QTime();
    }

    int totalPauseSeconds = 0;
    QDateTime lastPause;
    for (const auto& ev : timerEvents) {
        if (ev.type == TimerEvent::Pause) {
            lastPause = ev.timestamp;
        } else if (ev.type == TimerEvent::Resume && lastPause.isValid()) {
            totalPauseSeconds += lastPause.secsTo(ev.timestamp);
            lastPause = QDateTime();
        }
    }

    if (lastPause.isValid()) {
        totalPauseSeconds += lastPause.secsTo(QDateTime::currentDateTime());
    }

    int extraBreak = (totalPauseSeconds >= minBreak)
        ? minBreak
        : (minBreak - totalPauseSeconds);

    QDateTime finish = firstStart.addSecs(requiredWork + extraBreak);
    return finish.time();
}

void MainWindowState::updateFinishTime() {
    emit finishTimeChanged(calculateFinishTime());
}

