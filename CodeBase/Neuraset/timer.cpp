#include "timer.h"

Timer::Timer(QObject* parent) : QObject(parent), timer(new QTimer(this)), eventLoop(nullptr), interrupted(false) {
    connectTimer();
}

void Timer::connectTimer(){
    connect(timer, &QTimer::timeout, this, &Timer::handleTimeout);//Connect timer to timeout
}

//Main function, wait for sepecified number of seconds with event loop
void Timer::waitSeconds(int seconds) {
    eventLoop = new QEventLoop();
    interrupted = false;
    timer->start(seconds * 1000); // Convert seconds to milliseconds
    eventLoop->exec(); // Block the calling thread
    delete eventLoop;
    eventLoop = nullptr;
}

//Secondary function, waits for a specific number of milliseconds with event loop
void Timer::waitMilliseconds(int milliseconds){
    eventLoop = new QEventLoop();
    interrupted = false;
    timer->start(milliseconds); // Using milliseconds input
    eventLoop->exec(); // Block the calling thread
    delete eventLoop;
    eventLoop = nullptr;
}

//Interrupt timer if running
void Timer::interrupt() {
    if (eventLoop) {
        interrupted = true;
        eventLoop->exit();
    }
}

//receive signal at end of timer
void Timer::handleTimeout() {
    if (eventLoop && !interrupted) {
        eventLoop->exit();
    }
}
//simple function to convert seconds into minute:seconds and return string
std::string Timer::formatTime(int seconds){
    int minutes = seconds / 60;
           seconds %= 60;

           std::stringstream timestring;
           timestring << std::setfill('0') << std::setw(2) << minutes
              << ":" << std::setfill('0') << std::setw(2) << seconds;
           return timestring.str();
    }
