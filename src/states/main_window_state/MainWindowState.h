#pragma once
#include <QObject>
#include <QTimer>
#include <QTime>

class MainWindowState : public QObject {
    Q_OBJECT

    public:
        enum TimerStatus { Stopped, Running, Paused };

        explicit MainWindowState(QObject* parent = nullptr);

        TimerStatus getStatus() const;
        int getValue() const;

    signals:
        void timerStatusChanged(TimerStatus);
        void timerValueChanged(int);

    public slots:
        void setTimeStatus(TimerStatus status);

    private slots:
        void updateValue();

    private:
        void setTimerValue(int value);

        TimerStatus timerStatus;
        int timerValue = 0;
        QTime startTime;
        int elapsedBeforePause = 0;
        QTimer* timer;
        void start();
        void pause();
        void resume();
        void stop();
};
