#include "MainWindowState.h"

MainWindowState::MainWindowState(QObject* parent) : QObject(parent), timerStatus(Stopped), timerValue(0)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindowState::updateValue);
}

void MainWindowState::setTimeStatus(TimerStatus status) {
    if (status == Running) start();
    else if (status == Paused) pause();
    else stop();
}

void MainWindowState::start() {
    if (timerStatus != Running) {
        timerStatus = Running;
        emit timerStatusChanged(timerStatus);
        startTime = QTime::currentTime();
        timer->start(1000);
        logEvent(TimerEvent::Start);
    }
}

void MainWindowState::pause() {
    if (timerStatus == Running) {
        timerStatus = Paused;
        elapsedBeforePause += startTime.secsTo(QTime::currentTime());
        timer->stop();
        emit timerStatusChanged(timerStatus);
        logEvent(TimerEvent::Pause);
    }
}

void MainWindowState::resume() {
    if (timerStatus == Paused) {
        timerStatus = Running;
        startTime = QTime::currentTime();
        timer->start(1000);
        emit timerStatusChanged(timerStatus);
        logEvent(TimerEvent::Resume);
    }
}

void MainWindowState::stop() {
    timerStatus = Stopped;
    timer->stop();
    elapsedBeforePause = 0;
    emit timerStatusChanged(timerStatus);
    setTimerValue(0);
    logEvent(TimerEvent::Stop);
}

MainWindowState::TimerStatus MainWindowState::getStatus() const {
    return timerStatus;
}

int MainWindowState::getValue() const {
    return timerValue;
}

void MainWindowState::updateValue() {
    int elapsed = elapsedBeforePause + startTime.secsTo(QTime::currentTime());
    setTimerValue(elapsed);
}

void MainWindowState::setTimerValue(int value) {
    if (timerValue != value) {
        timerValue = value;
        emit timerValueChanged(timerValue);
    }
}

void MainWindowState::logEvent(TimerEvent::EventType type) {
    TimerEvent event{ type, QDateTime::currentDateTime() };
    timerEvents.append(event);
}
