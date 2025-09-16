#pragma once
#include <QWidget>
#include <QPushButton>
#include "states/main_window_state/MainWindowState.h"

class TrayPanel : public QWidget {
    Q_OBJECT
public:
    explicit TrayPanel(MainWindowState* state, QWidget* parent = nullptr);

private:
    QPushButton* startButton;
    QPushButton* pauseButton;
    QPushButton* resumeButton;
    QPushButton* stopButton;
    MainWindowState* windowState;

private slots:
    void updateButtonStates(MainWindowState::TimerStatus status);
};
