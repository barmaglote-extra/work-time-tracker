#include "TimerChart.h"

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
    onTimerValueChanged(windowState->getValue());
}

void TimerChart::onTimerValueChanged(int seconds) {
    updateProgressBar(seconds);
}

void TimerChart::updateProgressBar(int seconds) {
    int totalSeconds = windowState ? windowState->getTotalSeconds() : 1;
    double percent = (static_cast<double>(seconds) / totalSeconds) * 100.0;
    if (percent > 100.0) percent = 100.0;

    progressBar->setValue(static_cast<int>(percent));

    QString colorStart, colorEnd;
    if (percent <= 90.0) {
        colorStart = "#ff4d4d";
        colorEnd   = "#ff7f7f";
    } else if (percent <= 99.0) {
        colorStart = "#ffcc00";
        colorEnd   = "#ffdd66";
    } else {
        colorStart = "#4caf50";
        colorEnd   = "#81c784";
    }

    progressBar->setStyleSheet(progressBarStyle(colorStart, colorEnd));
}
