#include "components/timer_widget/TimerWidget.h"
#include "states/main_window_state/MainWindowState.h"
#include <QTime>

TimerWidget::TimerWidget(QWidget* parent, MainWindowState* state) : QWidget(parent), windowState(state) {
    layout = new QVBoxLayout(this);
    timeLabel = new QLabel("00:00", this);
    layout->addWidget(timeLabel, 0, Qt::AlignCenter);
    if (windowState) {
        connect(windowState, &MainWindowState::timerValueChanged,
                this, &TimerWidget::onValueChanged);
        connect(windowState, &MainWindowState::timerStatusChanged,
                this, &TimerWidget::onStatusChanged);
    }
}

void TimerWidget::onStatusChanged(MainWindowState::TimerStatus /*status*/) {
    // Change UI
}

void TimerWidget::onValueChanged(int seconds) {
    QTime t(0, 0);
    t = t.addSecs(seconds);
    timeLabel->setText(t.toString("mm:ss"));
}