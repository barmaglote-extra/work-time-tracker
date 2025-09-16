#include "components/control_panel/ControlPanel.h"
#include <QHBoxLayout>
#include "styles/ButtonStyles.h"

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {
    m_start = new QPushButton();
    m_pause = new QPushButton();
    m_resume = new QPushButton();
    m_stop = new QPushButton();

    m_start->setIcon(QIcon(":/res/resources/icons/start.svg"));
    m_pause->setIcon(QIcon(":/res/resources/icons/pause.svg"));
    m_resume->setIcon(QIcon(":/res/resources/icons/resume.svg"));
    m_stop->setIcon(QIcon(":/res/resources/icons/stop.svg"));

    m_start->setIconSize(QSize(32,32));
    m_pause->setIconSize(QSize(32,32));
    m_resume->setIconSize(QSize(32,32));
    m_stop->setIconSize(QSize(32,32));

    m_start->setStyleSheet(getButtonStyle());
    m_pause->setStyleSheet(getButtonStyle());
    m_resume->setStyleSheet(getButtonStyle());
    m_stop->setStyleSheet(getButtonStyle());

    auto* layout = new QHBoxLayout(this);
    layout->addWidget(m_start);
    layout->addWidget(m_pause);
    layout->addWidget(m_resume);
    layout->addWidget(m_stop);
}

void ControlPanel::setState(MainWindowState* state) {
    if (!state) return;

    m_state = state;

    connect(m_start, &QPushButton::clicked, [this]() { m_state->setTimeStatus(MainWindowState::TimerStatus::Running); });
    connect(m_pause, &QPushButton::clicked, [this]() { m_state->setTimeStatus(MainWindowState::TimerStatus::Paused); });
    connect(m_resume, &QPushButton::clicked, [this]() { m_state->setTimeStatus(MainWindowState::TimerStatus::Resumed); });
    connect(m_stop, &QPushButton::clicked, [this]() { m_state->setTimeStatus(MainWindowState::TimerStatus::Stopped); });

    connect(m_state, &MainWindowState::timerStatusChanged, this, &ControlPanel::updateButtonStates);
    updateButtonStates(m_state->getStatus());
}

void ControlPanel::updateButtonStates(MainWindowState::TimerStatus status) {
    switch (status) {
        case MainWindowState::TimerStatus::Running:
            m_start->setEnabled(false);
            m_resume->setEnabled(false);
            m_pause->setEnabled(true);
            m_stop->setEnabled(true);
            break;
        case MainWindowState::TimerStatus::Paused:
            m_start->setEnabled(false);
            m_resume->setEnabled(true);
            m_pause->setEnabled(false);
            m_stop->setEnabled(false);
            break;
        case MainWindowState::TimerStatus::Stopped:
            m_start->setEnabled(true);
            m_resume->setEnabled(false);
            m_pause->setEnabled(false);
            m_stop->setEnabled(false);
            break;
        case MainWindowState::TimerStatus::Resumed:
            m_start->setEnabled(false);
            m_resume->setEnabled(false);
            m_pause->setEnabled(true);
            m_stop->setEnabled(true);
            break;
    }
}
