#include "components/timer_widget/TimerWidget.h"
#include <QTime>
#include <cmath>
#include "utils/TimeCalculator.h"
#include "states/main_window_state/MainWindowState.h"
#include "models/TimerEvent.h"
#include <QApplication>
#include <QMessageBox>

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
    // Change UI
}

void TimerWidget::onValueChanged(int seconds) {
    if (!windowState) return;
    
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

void TimerWidget::updateRemainingTime() {
    if (!windowState) return;
    
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