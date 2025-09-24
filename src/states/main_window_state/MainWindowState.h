#pragma once
#include <QObject>
#include <QTimer>
#include <QTime>
#include <QMap>
#include <QDate>
#include <QVector>
#include "models/TimerEvent.h"

class MainWindowState : public QObject {
    Q_OBJECT

public:
    enum TimerStatus { Running, Paused, Resumed, Stopped };

    explicit MainWindowState(QObject* parent = nullptr);

    void setTimeStatus(TimerStatus status);
    void start();
    void pause();
    void resume();
    void stop();

    QTime getStartTime() const;
    QTime getFinishTime() const;
    QTime calculateFinishTime() const;

    TimerStatus getStatus() const;
    int getValue() const;
    int getTotalSeconds() const;

    const QVector<TimerEvent>& getTimerEvents() const { return timerEvents; }
    
    // Add the missing method for DailyChartWidget
    const QMap<QDate, int>& getDailyWorkDurations() const { return dailyWorkDurations; }

    void setWorkSecondsForDay(int day, int seconds);
    void setMinBreakSecondsForDay(int day, int seconds);

    const QMap<int, int>& getWorkSecondsPerDay() const { return workSecondsPerDay; }
    const QMap<int, int>& getMinBreakSecondsPerDay() const { return minBreakSecondsPerDay; }

    bool saveToFile(const QString& fileName) const;
    bool loadFromFile(const QString& fileName);
    void loadSettings(const QString& fileName);

    bool removePauseResumePair(int pauseRow);
    void updateStartTime(const QTime& newStartTime);
    void updateFinishTime(const QTime& newFinishTime);
    void updateFinishTime(); // Overload to recalculate and emit signal

    // Notification methods
    void checkWorkdayEnd();
    bool isWorkdayEnded(); // Remove const qualifier

signals:
    void timerValueChanged(int seconds);
    void timerStatusChanged(TimerStatus status);
    void finishTimeChanged(const QTime& finishTime);
    void workdayEnded(); // Signal for when workday ends

private slots:
    void updateValue();

private:
    void setTimerValue(int value);
    void logEvent(TimerEvent::EventType type);

    void recordDayDuration(const QDate& date, int seconds);
    void pruneOldDurations();
    void saveDailyDurations() const;
    void loadDailyDurations();

    QTimer* timer;
    QTimer* autosaveTimer;
    QTime startTime;
    int elapsedBeforePause;
    TimerStatus timerStatus;
    int timerValue;

    QVector<TimerEvent> timerEvents;

    // Work time settings per day (1-7, Monday-Sunday)
    QMap<int, int> workSecondsPerDay;     // Required work seconds per day
    QMap<int, int> minBreakSecondsPerDay; // Minimum break seconds per day

    // Daily work durations
    QMap<QDate, int> dailyWorkDurations;

    // Notification tracking
    bool workdayEndNotified;
};