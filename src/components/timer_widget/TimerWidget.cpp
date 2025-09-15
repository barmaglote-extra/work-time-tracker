#include "components/timer_widget/TimerWidget.h"
#include "states/main_window_state/MainWindowState.h"
#include <QTime>
#include <cmath>

TimerWidget::TimerWidget(QWidget* parent, MainWindowState* state) : QWidget(parent), windowState(state) {
    layout = new QVBoxLayout(this);

    timeLabel = new QLabel("00:00:00", this);
    QFont bigFont = timeLabel->font();
    bigFont.setPointSize(24);
    bigFont.setBold(true);
    timeLabel->setFont(bigFont);
    timeLabel->setAlignment(Qt::AlignCenter);

    int remainingSeconds = std::abs(windowState->getTotalSeconds());
    QTime leftTime(0, 0);
    leftTime = leftTime.addSecs(remainingSeconds);
    leftLabel = new QLabel(leftTime.toString("hh:mm:ss"), this);

    QFont smallFont = leftLabel->font();
    smallFont.setPointSize(14);
    smallFont.setBold(false);
    leftLabel->setFont(smallFont);
    leftLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(timeLabel);
    layout->addWidget(leftLabel);

    if (windowState) {
        connect(windowState, &MainWindowState::timerValueChanged, this, &TimerWidget::onValueChanged);
        connect(windowState, &MainWindowState::timerStatusChanged, this, &TimerWidget::onStatusChanged);
    }
}

void TimerWidget::onStatusChanged(MainWindowState::TimerStatus status) {
    // Change UI
}

void TimerWidget::onValueChanged(int seconds) {
    QTime currentTime(0, 0);
    currentTime = currentTime.addSecs(seconds);
    timeLabel->setText(currentTime.toString("hh:mm:ss"));

    int remainingSeconds = std::abs(windowState->getTotalSeconds() - seconds);;
    QTime leftTime(0, 0);
    leftTime = leftTime.addSecs(remainingSeconds);
    leftLabel->setText( leftTime.toString("hh:mm:ss"));

    if ((windowState->getTotalSeconds() - seconds) > 0) {
        leftLabel->setStyleSheet("color: red;");
    } else {
        leftLabel->setStyleSheet("color: green;");
    }
}