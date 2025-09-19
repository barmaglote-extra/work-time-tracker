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

        int extraBreak = (totalPauseSeconds >= minBreakSeconds) ? totalPauseSeconds : minBreakSeconds;
        int lackPauses = (totalPauseSeconds < minBreakSeconds) ? minBreakSeconds - totalPauseSeconds : 0;

        QDateTime finish = currentTime.addSecs(
            (requiredWorkSeconds - (firstStart.secsTo(currentTime) - totalPauseSeconds)) + lackPauses
        );

        return finish.time();
    }
}