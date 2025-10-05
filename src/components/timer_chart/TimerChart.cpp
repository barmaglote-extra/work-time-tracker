#include "TimerChart.h"
#include "utils/TimeCalculator.h"
#include <QDateTime>
#include <QTime>

TimerChart::TimerChart(QWidget* parent) : QWidget(parent), windowState(nullptr) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setTextVisible(true);
    layout->addWidget(progressBar);
}

void TimerChart::setState(MainWindowState* state) {
    if (!state) return;

    windowState = state;
    connect(windowState, &MainWindowState::timerValueChanged, this, &TimerChart::onTimerValueChanged);
    connect(windowState, &MainWindowState::timerStatusChanged, this, &TimerChart::onTimerStatusChanged);
    onTimerValueChanged(windowState->getValue());
}

void TimerChart::onTimerValueChanged(int seconds) {
    updateProgressBar(seconds);
}

void TimerChart::onTimerStatusChanged(MainWindowState::TimerStatus status) {
    // Force an update when timer status changes to ensure correct progress bar display
    updateProgressBar(windowState->getValue());
}

void TimerChart::updateProgressBar(int seconds) {
    // Get the start time from the window state
    QDateTime startTime = TimeCalculator::findFirstStartTime(windowState->getTimerEvents());

    // If there's no start time, we can't calculate progress
    if (!startTime.isValid()) {
        progressBar->setValue(0);
        progressBar->setStyleSheet(progressBarStyle("#cccccc", "#eeeeee"));
        return;
    }

    // Check if timer is paused to show pause progress instead of work progress
    if (windowState->getStatus() == MainWindowState::Paused) {
        updatePauseProgressBar();
        return;
    }

    // Get current time
    QDateTime currentTime = QDateTime::currentDateTime();

    // Calculate end time using TimeCalculator
    int today = QDate::currentDate().dayOfWeek();
    int requiredWorkSeconds = windowState->getTotalSeconds();
    int minBreakSeconds = windowState->getMinBreakSecondsPerDay().value(today, 0);

    QTime finishTime = TimeCalculator::calculateFinishTime(
        windowState->getTimerEvents(),
        requiredWorkSeconds,
        minBreakSeconds,
        currentTime
    );

    QDateTime endTime = QDateTime(currentTime.date(), finishTime);

    // Calculate progress based on start time, end time, and current time
    qint64 totalDuration = startTime.secsTo(endTime);
    qint64 elapsedDuration = startTime.secsTo(currentTime);

    double percent = 0.0;
    if (totalDuration > 0) {
        percent = (static_cast<double>(elapsedDuration) / totalDuration) * 100.0;
    }

    // Cap the percentage at 100
    if (percent > 100.0) percent = 100.0;

    progressBar->setValue(static_cast<int>(percent));

    QString colorStart, colorEnd;
    if (percent <= 90.0) {
        colorStart = "#ff4d4d";
        colorEnd   = "#ff7f7f";
    } else if (percent < 100) {
        colorStart = "#ffcc00";
        colorEnd   = "#ffdd66";
    } else {
        colorStart = "#4caf50";
        colorEnd   = "#81c784";
    }

    progressBar->setStyleSheet(progressBarStyle(colorStart, colorEnd));
}

void TimerChart::updatePauseProgressBar() {
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

    // Calculate pause progress percentage
    double percent = 0.0;
    if (minBreakSeconds > 0) {
        percent = (static_cast<double>(totalPauseSeconds) / minBreakSeconds) * 100.0;
    }

    // Cap the percentage at 100
    if (percent > 100.0) percent = 100.0;

    progressBar->setValue(static_cast<int>(percent));

    // Set color based on pause progress
    QString colorStart, colorEnd;
    if (percent <= 50.0) {
        colorStart = "#4d94ff";  // Blue for low pause time
        colorEnd   = "#66a3ff";
    } else if (percent < 100) {
        colorStart = "#9966ff";  // Purple for medium pause time
        colorEnd   = "#b38fff";
    } else {
        colorStart = "#cc66ff";  // Violet for sufficient pause time
        colorEnd   = "#d999ff";
    }

    progressBar->setStyleSheet(progressBarStyle(colorStart, colorEnd));
}