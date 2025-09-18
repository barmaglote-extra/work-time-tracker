#include "DefaultStatusBar.h"
#include <QLabel>

void DefaultStatusBar::setupUIImpl() {
    label = new QLabel("Status: Idle", this);
    label->setFrameShape(QFrame::NoFrame);
    label->setFrameShadow(QFrame::Plain);

    this->setStyleSheet(
        "QStatusBar::item { border: none; padding: 2px; margin: 0px; }"
    );

    this->setSizeGripEnabled(true);

    addWidget(label, 1);
}

void DefaultStatusBar::setState(MainWindowState* state) {
    if (!state) return;

    windowState = state;

    connect(windowState, &MainWindowState::timerStatusChanged, this, &DefaultStatusBar::updateStatus);
    connect(windowState, &MainWindowState::timerValueChanged, this, &DefaultStatusBar::updateStatus);
    connect(windowState, &MainWindowState::finishTimeChanged, this, &DefaultStatusBar::updateStatus);

    updateStatus();
}

void DefaultStatusBar::updateStatus() {
    if (!windowState || !label) return;

    QString statusText;
    switch (windowState->getStatus()) {
        case MainWindowState::TimerStatus::Stopped:
            statusText = "Stopped";
            break;
        case MainWindowState::TimerStatus::Running:
            statusText = "Running";
            break;
        case MainWindowState::TimerStatus::Paused:
            statusText = "Paused";
            break;
        default:
            statusText = "Unknown";
            break;
    }

    label->setText("Status: " + statusText);
}