#pragma once

#include <QVector>
#include <QDateTime>
#include <QTime>
#include "models/TimerEvent.h"

namespace TimeCalculator {
    /**
     * @brief Calculate the finish time based on work events and requirements
     * This is a pure function that can be easily tested
     *
     * @param timerEvents Vector of timer events
     * @param requiredWorkSeconds Required work time in seconds
     * @param minBreakSeconds Minimum break time in seconds
     * @param currentTime Current time for calculation
     * @return Calculated finish time
     */
    QTime calculateFinishTime(
        const QVector<TimerEvent>& timerEvents,
        int requiredWorkSeconds,
        int minBreakSeconds,
        const QDateTime& currentTime);

    /**
     * @brief Calculate total pause time from timer events
     * This is a pure function that can be easily tested
     *
     * @param timerEvents Vector of timer events
     * @param currentTime Current time for calculating ongoing pauses
     * @return Total pause time in seconds
     */
    int calculateTotalPauseSeconds(
        const QVector<TimerEvent>& timerEvents,
        const QDateTime& currentTime);

    /**
     * @brief Find the first start time from timer events
     * This is a pure function that can be easily tested
     *
     * @param timerEvents Vector of timer events
     * @return DateTime of the first start event, or invalid if none found
     */
    QDateTime findFirstStartTime(const QVector<TimerEvent>& timerEvents);

    /**
     * @brief Find the last stop time from timer events
     * This is a pure function that can be easily tested
     *
     * @param timerEvents Vector of timer events
     * @return DateTime of the last stop event, or invalid if none found
     */
    QDateTime findLastStopTime(const QVector<TimerEvent>& timerEvents);

    /**
     * @brief Calculate the elapsed time including break adjustments
     * This ensures consistent time calculation across the application
     *
     * @param timerValue Raw timer value in seconds
     * @param timerEvents Vector of timer events
     * @param minBreakSeconds Minimum break time in seconds
     * @param currentTime Current time for calculation
     * @return Adjusted elapsed time
     */
    QTime calculateElapsedTime(
        int timerValue,
        const QVector<TimerEvent>& timerEvents,
        int minBreakSeconds,
        const QDateTime& currentTime);

    /**
     * @brief Calculate the remaining time until workday end
     * This ensures consistent time calculation across the application
     *
     * @param timerValue Raw timer value in seconds
     * @param requiredWorkSeconds Required work time in seconds
     * @param timerEvents Vector of timer events
     * @param minBreakSeconds Minimum break time in seconds
     * @param currentTime Current time for calculation
     * @return Remaining time until workday end
     */
    QTime calculateRemainingTime(
        int timerValue,
        int requiredWorkSeconds,
        const QVector<TimerEvent>& timerEvents,
        int minBreakSeconds,
        const QDateTime& currentTime);
}