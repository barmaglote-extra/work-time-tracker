#pragma once
#include <QObject>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QFile>
#include <QMap>
#include <QDate>
#include <QJsonArray>
#include <QString>
#include "models/TimerEvent.h"

class MainWindowState : public QObject {
    Q_OBJECT

    public:
        enum TimerStatus { Stopped, Running, Paused, Resumed };

        explicit MainWindowState(QObject* parent = nullptr);

        TimerStatus getStatus() const;
        int getValue() const;
        bool saveToFile(const QString& fileName) const;
        bool loadFromFile(const QString& fileName);
        int getTotalSeconds() const;

        QMap<int,int>& getWorkSecondsPerDay() { return workSecondsPerDay; }
        QMap<int,int>& getMinBreakSecondsPerDay() { return minBreakSecondsPerDay; }

        void updateFinishTime();

    public:
        QTime calculateFinishTime();
        void setWorkSecondsForDay(int day, int seconds);
        void setMinBreakSecondsForDay(int day, int seconds);

    signals:
        void timerStatusChanged(TimerStatus);
        void timerValueChanged(int);
        void finishTimeChanged(const QTime& finishTime);

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
        QTimer* autosaveTimer;

        void start();
        void pause();
        void resume();
        void stop();
        void setTimerValue(int value);
        void logEvent(TimerEvent::EventType type);
        QMap<int, int> workSecondsPerDay;      // key = 1..7 (Monday..Sunday)
        QMap<int, int> minBreakSecondsPerDay;
        void loadSettings(const QString& fileName);
};
