#include "components/timer_widget/TimerWidget.h"
#include <QTime>
#include <cmath>
#include "utils/TimeCalculator.h"

TimerWidget::TimerWidget(QWidget* parent) : QWidget(parent), windowState(nullptr) {
    layout = new QVBoxLayout(this);

    timeLabel = new QLabel("00:00:00", this);
    QFont bigFont = timeLabel->font();
    bigFont.setPointSize(24);
    bigFont.setBold(true);
    timeLabel->setFont(bigFont);
    timeLabel->setAlignment(Qt::AlignCenter);

    leftLabel = new QLabel("00:00:00", this);
    QFont smallFont = leftLabel->font();
    smallFont.setPointSize(14);
    smallFont.setBold(false);
    leftLabel->setFont(smallFont);
    leftLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(timeLabel);
    layout->addWidget(leftLabel);
}

void TimerWidget::setState(MainWindowState* state) {
    if (!state) return;

    windowState = state;

    connect(windowState, &MainWindowState::timerValueChanged, this, &TimerWidget::onValueChanged);
    connect(windowState, &MainWindowState::timerStatusChanged, this, &TimerWidget::onStatusChanged);
    connect(windowState, &MainWindowState::finishTimeChanged, this, &TimerWidget::onFinishTimeChanged);

    onValueChanged(windowState->getValue());
    onStatusChanged(windowState->getStatus());
    onFinishTimeChanged(windowState->calculateFinishTime());
}

void TimerWidget::onStatusChanged(MainWindowState::TimerStatus status) {
    // Change UI
}

void TimerWidget::onValueChanged(int seconds) {
    QTime currentTime(0, 0);
    currentTime = currentTime.addSecs(seconds);

    int todayOfWeek = QDate::currentDate().dayOfWeek();
    int minBreakSeconds = windowState->getMinBreakSecondsPerDay().value(todayOfWeek, 0);

    // Calculate total pause seconds for today
    const auto& events = windowState->getTimerEvents();
    QDate today = QDate::currentDate();
    QVector<TimerEvent> todayEvents;
    for (const auto& event : events) {
        if (event.timestamp.date() == today) {
            todayEvents.append(event);
        }
    }

    int totalPauseSeconds = TimeCalculator::calculateTotalPauseSeconds(
        todayEvents,
        QDateTime::currentDateTime()
    );

    currentTime = currentTime.addSecs(totalPauseSeconds > minBreakSeconds ? minBreakSeconds : totalPauseSeconds);

    timeLabel->setText(currentTime.toString("hh:mm:ss"));

    // Also update the remaining time whenever the timer value changes
    updateRemainingTime();
}

void TimerWidget::onFinishTimeChanged(const QTime& finishTime) {
    // Update the remaining time when finish time changes
    updateRemainingTime();
}

void TimerWidget::updateRemainingTime() {
    if (!windowState) return;

    // Calculate finish time using TimeCalculator
    QTime finishTime = windowState->calculateFinishTime();

    // Calculate remaining seconds until finish time
    QTime now = QTime::currentTime();
    int remainingSeconds = now.secsTo(finishTime);

    // Handle case where finish time has passed

    if (remainingSeconds < 0 && windowState->getStatus() == MainWindowState::TimerStatus::Stopped) {
        remainingSeconds = 0;
    }

    QTime leftTime(0, 0);
    leftTime = leftTime.addSecs(abs(remainingSeconds));
    leftLabel->setText(leftTime.toString("hh:mm:ss"));

    // Set color based on whether we're ahead or behind schedule
    // Get today's day of week (1-7, Monday-Sunday)
    int todayOfWeek = QDate::currentDate().dayOfWeek();
    int requiredWorkSeconds = windowState->getTotalSeconds();
    int minBreakSeconds = windowState->getMinBreakSecondsPerDay().value(todayOfWeek, 0);

    // Calculate total pause seconds for today
    const auto& events = windowState->getTimerEvents();
    QDate today = QDate::currentDate();
    QVector<TimerEvent> todayEvents;
    for (const auto& event : events) {
        if (event.timestamp.date() == today) {
            todayEvents.append(event);
        }
    }

    int totalPauseSeconds = TimeCalculator::calculateTotalPauseSeconds(
        todayEvents,
        QDateTime::currentDateTime()
    );

    // Calculate the total required time (work + required breaks)
    int requiredTotalSeconds = requiredWorkSeconds + minBreakSeconds;
    int actualTotalSeconds = windowState->getValue() + totalPauseSeconds;

    // Compare actual time spent with required time
    if (actualTotalSeconds < requiredTotalSeconds) {
        leftLabel->setStyleSheet("color: red;");
    } else {
        leftLabel->setStyleSheet("color: green;");
    }
}