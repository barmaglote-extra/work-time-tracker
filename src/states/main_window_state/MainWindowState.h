#pragma once
#include <QObject>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QFile>
#include <QJsonArray>
#include "models/TimerEvent.h"

class MainWindowState : public QObject {
    Q_OBJECT

    public:
        enum TimerStatus { Stopped, Running, Paused, Resumed };

        explicit MainWindowState(QObject* parent = nullptr);

        TimerStatus getStatus() const;
        int getValue() const;
        int getTotalSeconds() const { return totalSeconds; }
        void setTotalSeconds(int seconds) { totalSeconds = seconds; }
        bool saveToFile(const QString& fileName) const;
        bool loadFromFile(const QString& fileName);

    signals:
        void timerStatusChanged(TimerStatus);
        void timerValueChanged(int);

    public slots:
        void setTimeStatus(TimerStatus status);

    private slots:
        void updateValue();

    private:
        QVector<TimerEvent> timerEvents;
        TimerStatus timerStatus = Stopped;
        int timerValue = 0;
        QTime startTime;
        int elapsedBeforePause = 0;
        QTimer* timer;
        int totalSeconds = 100; // 9 * 60 * 60; // default 9 часов
        QTimer* autosaveTimer;

        void start();
        void pause();
        void resume();
        void stop();
        void setTimerValue(int value);
        void logEvent(TimerEvent::EventType type);
};
