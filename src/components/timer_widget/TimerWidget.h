#pragma once
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTime>
#include "states/main_window_state/MainWindowState.h"

class TimerWidget : public QWidget {
    Q_OBJECT
    public:
        explicit TimerWidget(QWidget* parent = nullptr);
        void setState(MainWindowState* state);

    private slots:
        void onStatusChanged(MainWindowState::TimerStatus status);
        void onValueChanged(int seconds);
        void onFinishTimeChanged(const QTime& finishTime);

    private:
        QLabel* timeLabel;
        QLabel* leftLabel;
        QVBoxLayout* layout;
        MainWindowState* windowState;

        // New method to update remaining time
        void updateRemainingTime();
};