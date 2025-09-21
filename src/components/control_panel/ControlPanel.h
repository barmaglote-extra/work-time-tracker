#pragma once
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include "states/main_window_state/MainWindowState.h"

class ControlPanel : public QWidget {
    Q_OBJECT
    public:
        ControlPanel(QWidget* parent = nullptr);

        QPushButton* startButton() const { return startBtn; }
        QPushButton* pauseButton() const { return pauseBtn; }
        QPushButton* resumeButton() const { return resumeBtn; }
        QPushButton* stopButton() const { return stopBtn; }
        void setState(MainWindowState* state);

    public slots:
        void updateButtonStates(MainWindowState::TimerStatus status);

    private:
        MainWindowState* windowState;
        QPushButton *startBtn, *pauseBtn, *resumeBtn, *stopBtn;

        bool hasDataForCurrentDay() const;
};