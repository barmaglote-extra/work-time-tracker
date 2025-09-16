#pragma once
#include <QWidget>
#include <QPushButton>
#include "states/main_window_state/MainWindowState.h"

class ControlPanel : public QWidget {
    Q_OBJECT
    public:
        ControlPanel(QWidget* parent = nullptr);

        QPushButton* startButton() const { return m_start; }
        QPushButton* pauseButton() const { return m_pause; }
        QPushButton* resumeButton() const { return m_resume; }
        QPushButton* stopButton() const { return m_stop; }
        void setState(MainWindowState* state);

    public slots:
        void updateButtonStates(MainWindowState::TimerStatus status);

    private:
        MainWindowState* m_state;
        QPushButton *m_start, *m_pause, *m_resume, *m_stop;
};
