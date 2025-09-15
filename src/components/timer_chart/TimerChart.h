#pragma once
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include "states/main_window_state/MainWindowState.h"

class TimerChart : public QWidget {
    Q_OBJECT
public:
    explicit TimerChart(QWidget* parent = nullptr);
    void setState(MainWindowState* state);

    private slots:
        void onTimerValueChanged(int seconds);

    private:
        QLabel* titleLabel;
        QProgressBar* progressBar;
        MainWindowState* windowState;
        void updateProgressBar(int seconds);
};
