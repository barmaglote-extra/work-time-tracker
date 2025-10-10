#include "TimeCalculator.h"
#include <QVector>
#include <QDateTime>
#include <QTime>
#include <QDate>

namespace TimeCalculator {
    QDateTime findFirstStartTime(const QVector<TimerEvent>& timerEvents) {
        for (const auto& ev : timerEvents) {
            if (ev.type == TimerEvent::Start) {
                return ev.timestamp;
            }
        }
        return QDateTime();
    }

    int calculateTotalPauseSeconds(
        const QVector<TimerEvent>& timerEvents,
        const QDateTime& currentTime) {

        int totalPauseSeconds = 0;
        QDateTime lastPause;

        for (const auto& ev : timerEvents) {
            if (ev.type == TimerEvent::Pause) {
                lastPause = ev.timestamp;
            } else if (ev.type == TimerEvent::Resume && lastPause.isValid()) {
                totalPauseSeconds += lastPause.secsTo(ev.timestamp);
                lastPause = QDateTime();
            }
        }

        if (lastPause.isValid()) {
            totalPauseSeconds += lastPause.secsTo(currentTime);
        }

        return totalPauseSeconds;
    }

    QTime calculateFinishTime(
        const QVector<TimerEvent>& timerEvents,
        int requiredWorkSeconds,
        int minBreakSeconds,
        const QDateTime& currentTime) {

        // Guard clauses - early returns for edge cases
        if (timerEvents.isEmpty()) {
            return QTime();
        }

        QDateTime firstStart = findFirstStartTime(timerEvents);
        if (!firstStart.isValid()) {
            return QTime();
        }

        int totalPauseSeconds = calculateTotalPauseSeconds(timerEvents, currentTime);
        int lackPauses = (totalPauseSeconds < minBreakSeconds) ? minBreakSeconds - totalPauseSeconds : 0;

        QDateTime finish = currentTime.addSecs(
            (requiredWorkSeconds - (firstStart.secsTo(currentTime) - totalPauseSeconds)) + lackPauses
        );

        return finish.time();
    }

    QDateTime findLastStopTime(const QVector<TimerEvent>& timerEvents) {
        // Iterate backwards through the events to find the last stop event
        for (int i = timerEvents.size() - 1; i >= 0; i--) {
            if (timerEvents[i].type == TimerEvent::Stop) {
                return timerEvents[i].timestamp;
            }
        }
        return QDateTime();
    }

    QTime calculateElapsedTime(
        int timerValue,
        const QVector<TimerEvent>& timerEvents,
        int minBreakSeconds,
        const QDateTime& currentTime) {

        QTime currentTimeObj(0, 0);
        currentTimeObj = currentTimeObj.addSecs(timerValue);

        // Calculate total pause seconds for today
        int totalPauseSeconds = calculateTotalPauseSeconds(timerEvents, currentTime);

        // Apply the same adjustment as in TimerWidget
        currentTimeObj = currentTimeObj.addSecs(totalPauseSeconds > minBreakSeconds ? minBreakSeconds : totalPauseSeconds);

        return currentTimeObj;
    }

    QTime calculateRemainingTime(
        int timerValue,
        int requiredWorkSeconds,
        const QVector<TimerEvent>& timerEvents,
        int minBreakSeconds,
        const QDateTime& currentTime) {

        // Calculate finish time using TimeCalculator
        QTime finishTime = calculateFinishTime(timerEvents, requiredWorkSeconds, minBreakSeconds, currentTime);

        // Calculate remaining seconds until finish time
        int remainingSeconds = currentTime.time().secsTo(finishTime);

        // Handle case where finish time has passed
        if (remainingSeconds < 0) {
            remainingSeconds = 0;
        }

        QTime leftTime(0, 0);
        leftTime = leftTime.addSecs(abs(remainingSeconds));
        return leftTime;
    }
}