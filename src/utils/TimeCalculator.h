#pragma once
#include <QVector>
#include <QDateTime>
#include <QTime>
#include "../models/TimerEvent.h"

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
}