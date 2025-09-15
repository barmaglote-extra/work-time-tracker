#include "TimerChart.h"

TimerChart::TimerChart(QWidget* parent) : QWidget(parent), windowState(nullptr) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    titleLabel = new QLabel("Progress", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setTextVisible(true);

    layout->addWidget(titleLabel);
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

    QString color;
    if (percent <= 90.0) color = "red";
    else if (percent <= 99.0) color = "yellow";
    else color = "green";

    progressBar->setStyleSheet(QString(
        "QProgressBar { border: 1px solid #bbb; border-radius: 5px; text-align: center; }"
        "QProgressBar::chunk { background-color: %1; border-radius: 5px; }"
    ).arg(color));
}
