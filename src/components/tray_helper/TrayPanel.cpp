#include "TrayPanel.h"
#include <QHBoxLayout>
#include "styles/ButtonStyles.h"

TrayPanel::TrayPanel(MainWindowState* state, QWidget* parent)
    : QWidget(parent), windowState(state)
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    startButton = new QPushButton();
    pauseButton = new QPushButton();
    resumeButton = new QPushButton();
    stopButton = new QPushButton();

    startButton->setIcon(QIcon(":/res/resources/icons/start.svg"));
    pauseButton->setIcon(QIcon(":/res/resources/icons/pause.svg"));
    resumeButton->setIcon(QIcon(":/res/resources/icons/resume.svg"));
    stopButton->setIcon(QIcon(":/res/resources/icons/stop.svg"));

    startButton->setIconSize(QSize(24,24));
    pauseButton->setIconSize(QSize(24,24));
    resumeButton->setIconSize(QSize(24,24));
    stopButton->setIconSize(QSize(24,24));

    startButton->setStyleSheet(getButtonStyle(35, 14));
    pauseButton->setStyleSheet(getButtonStyle(35, 14));
    resumeButton->setStyleSheet(getButtonStyle(35, 14));
    stopButton->setStyleSheet(getButtonStyle(35, 14));

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(5,5,5,5);
    layout->addWidget(startButton);
    layout->addWidget(pauseButton);
    layout->addWidget(resumeButton);
    layout->addWidget(stopButton);

    connect(startButton, &QPushButton::clicked, [this]() { windowState->setTimeStatus(MainWindowState::TimerStatus::Running); });
    connect(pauseButton, &QPushButton::clicked, [this]() { windowState->setTimeStatus(MainWindowState::TimerStatus::Paused); });
    connect(resumeButton, &QPushButton::clicked, [this]() { windowState->setTimeStatus(MainWindowState::TimerStatus::Resumed); });
    connect(stopButton, &QPushButton::clicked, [this]() { windowState->setTimeStatus(MainWindowState::TimerStatus::Stopped); });

    connect(windowState, &MainWindowState::timerStatusChanged, this, &TrayPanel::updateButtonStates);
    updateButtonStates(windowState->getStatus());
}

void TrayPanel::updateButtonStates(MainWindowState::TimerStatus status) {
    switch (status) {
        case MainWindowState::TimerStatus::Running:
            startButton->setEnabled(false);
            resumeButton->setEnabled(false);
            pauseButton->setEnabled(true);
            stopButton->setEnabled(true);
            break;

        case MainWindowState::TimerStatus::Paused:
            startButton->setEnabled(false);
            resumeButton->setEnabled(true);
            pauseButton->setEnabled(false);
            stopButton->setEnabled(false);
            break;

        case MainWindowState::TimerStatus::Stopped:
            startButton->setEnabled(true);
            resumeButton->setEnabled(false);
            pauseButton->setEnabled(false);
            stopButton->setEnabled(false);
            break;

        case MainWindowState::TimerStatus::Resumed:
            startButton->setEnabled(false);
            resumeButton->setEnabled(false);
            pauseButton->setEnabled(true);
            stopButton->setEnabled(true);
            break;
    }
}
