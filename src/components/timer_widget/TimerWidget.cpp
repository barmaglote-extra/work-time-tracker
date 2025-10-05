#include "components/timer_widget/TimerWidget.h"
#include <QTime>
#include <cmath>
#include "utils/TimeCalculator.h"
#include "states/main_window_state/MainWindowState.h"
#include "models/TimerEvent.h"
#include <QApplication>
#include <QMessageBox>
#include <QDate>
#include <QDateTime>

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

    onValueChanged(windowState->getValue());
    onStatusChanged(windowState->getStatus());
}

void TimerWidget::onStatusChanged(MainWindowState::TimerStatus status) {
    // When status changes, update the display to show appropriate information
    // If paused, show pause time information; otherwise show normal work time
    updatePauseTimeDisplay();
}

void TimerWidget::onValueChanged(int seconds) {
    if (!windowState) return;

    // Check if timer is paused to show pause time instead of work time
    if (windowState->getStatus() == MainWindowState::Paused) {
        updatePauseTimeDisplay();
        return;
    }

    // Use TimeCalculator for consistent elapsed time calculation
    QVector<TimerEvent> timerEvents = windowState->getTimerEvents();
    int todayOfWeek = QDate::currentDate().dayOfWeek();
    int minBreakSeconds = windowState->getMinBreakSecondsPerDay().value(todayOfWeek, 0);
    QDateTime currentTime = QDateTime::currentDateTime();

    QTime elapsedTime = TimeCalculator::calculateElapsedTime(
        seconds, timerEvents, minBreakSeconds, currentTime);

    timeLabel->setText(elapsedTime.toString("hh:mm:ss"));

    // Also update the remaining time whenever the timer value changes
    updateRemainingTime();
}

void TimerWidget::onFinishTimeChanged(const QTime& finishTime) {
    // Update the remaining time when finish time changes
    updateRemainingTime();
}

void TimerWidget::updatePauseTimeDisplay() {
    if (!windowState) return;

    // Only update pause display if timer is actually paused
    if (windowState->getStatus() != MainWindowState::Paused) {
        return;
    }

    // Get current time
    QDateTime currentTime = QDateTime::currentDateTime();

    // Calculate total pause time using TimeCalculator
    int totalPauseSeconds = TimeCalculator::calculateTotalPauseSeconds(
        windowState->getTimerEvents(),
        currentTime
    );

    // Get minimum break seconds for today
    int today = QDate::currentDate().dayOfWeek();
    int minBreakSeconds = windowState->getMinBreakSecondsPerDay().value(today, 0);

    // Convert pause time to QTime for display
    QTime pauseTime(0, 0, 0);
    pauseTime = pauseTime.addSecs(totalPauseSeconds);
    timeLabel->setText(pauseTime.toString("hh:mm:ss"));

    // Calculate remaining pause time
    int remainingPauseSeconds = minBreakSeconds - totalPauseSeconds;
    if (remainingPauseSeconds < 0) remainingPauseSeconds = 0;

    QTime remainingTime(0, 0, 0);
    remainingTime = remainingTime.addSecs(remainingPauseSeconds);
    leftLabel->setText(remainingTime.toString("hh:mm:ss"));

    // Set color based on pause progress
    if (totalPauseSeconds >= minBreakSeconds) {
        // Sufficient pause time
        timeLabel->setStyleSheet("color: green;");
        leftLabel->setStyleSheet("color: green;");
    } else {
        // Still need more pause time
        timeLabel->setStyleSheet("color: blue;");
        leftLabel->setStyleSheet("color: blue;");
    }
}

void TimerWidget::updateRemainingTime() {
    if (!windowState) return;

    // Don't update remaining time if timer is paused (handled by updatePauseTimeDisplay)
    if (windowState->getStatus() == MainWindowState::Paused) {
        return;
    }

    int seconds = windowState->getValue();
    int total = windowState->getTotalSeconds();

    // Use TimeCalculator for consistent remaining time calculation
    QVector<TimerEvent> timerEvents = windowState->getTimerEvents();
    int todayOfWeek = QDate::currentDate().dayOfWeek();
    int minBreakSeconds = windowState->getMinBreakSecondsPerDay().value(todayOfWeek, 0);
    QDateTime currentTime = QDateTime::currentDateTime();

    QTime remainingTime = TimeCalculator::calculateRemainingTime(
        seconds, total, timerEvents, minBreakSeconds, currentTime);

    leftLabel->setText(remainingTime.toString("hh:mm:ss"));

    // Set color based on whether we're ahead or behind schedule
    // Get today's day of week (1-7, Monday-Sunday)
    int requiredWorkSeconds = windowState->getTotalSeconds();

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