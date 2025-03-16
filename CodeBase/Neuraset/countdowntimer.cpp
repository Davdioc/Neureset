#include "countdowntimer.h"

CountdownTimer::CountdownTimer(int duration, QObject *parent)
    : QObject(parent), remainingTime(duration) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CountdownTimer::updateTimer);

    connect(this, &CountdownTimer::startTimer, this, &CountdownTimer::startCountdown); // Updated
    connect(this, &CountdownTimer::stopTimer, this, &CountdownTimer::stopCountdown);   // Updated
}

bool CountdownTimer::isRunning() const {
    return timer->isActive();
}


void CountdownTimer::startCountdown() {
    if (!isRunning()) {
        if (remainingTime == 0) {           // Checks if the timer has finished or is being started for the first time.
            resetTimer();         // Only reset if the timer has finished or not set.
        }
        timer->start(1000);                 // Start or resume the timer.
    }
}


void CountdownTimer::stopCountdown() {
    timer->stop();
}

void CountdownTimer::resetTimer() {
    remainingTime = 32;             // Reset the countdown time.
}

void CountdownTimer::updateTimer(){
    if (remainingTime > 0) {
        --remainingTime;
        emit countdownUpdated(remainingTime);
    } else {
        timer->stop();
    }
}

