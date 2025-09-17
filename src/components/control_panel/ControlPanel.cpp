#include "components/control_panel/ControlPanel.h"
#include <QHBoxLayout>
#include "styles/ButtonStyles.h"

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {
    startBtn = new QPushButton();
    pauseBtn = new QPushButton();
    resumeBtn = new QPushButton();
    stopBtn = new QPushButton();

    startBtn->setIcon(QIcon(":/res/resources/icons/start.svg"));
    pauseBtn->setIcon(QIcon(":/res/resources/icons/pause.svg"));
    resumeBtn->setIcon(QIcon(":/res/resources/icons/resume.svg"));
    stopBtn->setIcon(QIcon(":/res/resources/icons/stop.svg"));

    startBtn->setIconSize(QSize(32,32));
    pauseBtn->setIconSize(QSize(32,32));
    resumeBtn->setIconSize(QSize(32,32));
    stopBtn->setIconSize(QSize(32,32));

    startBtn->setStyleSheet(getButtonStyle());
    pauseBtn->setStyleSheet(getButtonStyle());
    resumeBtn->setStyleSheet(getButtonStyle());
    stopBtn->setStyleSheet(getButtonStyle());

    auto* layout = new QHBoxLayout(this);
    layout->addWidget(startBtn);
    layout->addWidget(pauseBtn);
    layout->addWidget(resumeBtn);
    layout->addWidget(stopBtn);
}

void ControlPanel::setState(MainWindowState* state) {
    if (!state) return;

    windowState = state;

    connect(startBtn, &QPushButton::clicked, [this]() { windowState->setTimeStatus(MainWindowState::TimerStatus::Running); });
    connect(pauseBtn, &QPushButton::clicked, [this]() { windowState->setTimeStatus(MainWindowState::TimerStatus::Paused); });
    connect(resumeBtn, &QPushButton::clicked, [this]() { windowState->setTimeStatus(MainWindowState::TimerStatus::Resumed); });
    connect(stopBtn, &QPushButton::clicked, [this]() { windowState->setTimeStatus(MainWindowState::TimerStatus::Stopped); });

    connect(windowState, &MainWindowState::timerStatusChanged, this, &ControlPanel::updateButtonStates);
    updateButtonStates(windowState->getStatus());
}

void ControlPanel::updateButtonStates(MainWindowState::TimerStatus status) {
    switch (status) {
        case MainWindowState::TimerStatus::Running:
            startBtn->setEnabled(false);
            resumeBtn->setEnabled(false);
            pauseBtn->setEnabled(true);
            stopBtn->setEnabled(true);
            break;
        case MainWindowState::TimerStatus::Paused:
            startBtn->setEnabled(false);
            resumeBtn->setEnabled(true);
            pauseBtn->setEnabled(false);
            stopBtn->setEnabled(false);
            break;
        case MainWindowState::TimerStatus::Stopped:
            startBtn->setEnabled(true);
            resumeBtn->setEnabled(false);
            pauseBtn->setEnabled(false);
            stopBtn->setEnabled(false);
            break;
        case MainWindowState::TimerStatus::Resumed:
            startBtn->setEnabled(false);
            resumeBtn->setEnabled(false);
            pauseBtn->setEnabled(true);
            stopBtn->setEnabled(true);
            break;
    }
}
