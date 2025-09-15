#include <QDateTime>
#include <QJsonObject>

struct TimerEvent {
    enum EventType { Start, Pause, Resume, Stop } type;
    QDateTime timestamp;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["timestamp"] = timestamp.toString(Qt::ISODate);
        return obj;
    }

    static TimerEvent fromJson(const QJsonObject& obj) {
        TimerEvent e;
        e.type = static_cast<EventType>(obj["type"].toInt());
        e.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
        return e;
    }
};