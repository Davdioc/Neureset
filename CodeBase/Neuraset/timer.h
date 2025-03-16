#ifndef TIMER_H_
#define TIMER_H_

#include <QObject>
#include <QTimer>
#include <QEventLoop>
#include "defs.h"
#include <string>
#include <sstream>
#include <iomanip>

class Timer : public QObject {
    Q_OBJECT
public:
    Timer(QObject* parent = nullptr);
    ~Timer(){};

    void waitSeconds(int seconds);//pause program for this many seconds
    void waitMilliseconds(int milliseconds);    //pause program for this many milliseconds
    void interrupt();//interrupt the timer
    std::string formatTime(int seconds);

private slots:
    void handleTimeout();//end timer

private:
    void connectTimer();//connect timeout slot
    QTimer* timer;
    QEventLoop* eventLoop;
    bool interrupted;
};

#endif // TIMER_H
