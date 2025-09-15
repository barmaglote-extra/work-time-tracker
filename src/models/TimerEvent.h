#include <QDateTime>

struct TimerEvent {
    enum EventType { Start, Pause, Resume, Stop } type;
    QDateTime timestamp;
};