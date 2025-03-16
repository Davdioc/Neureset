#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

#include <QObject>
#include <QTimer>

class CountdownTimer : public QObject {
    Q_OBJECT

public:
    explicit CountdownTimer(int duration, QObject *parent = nullptr);
    bool isRunning() const;

signals:
    void countdownUpdated(int secondsLeft);
    void startTimer();
    void stopTimer();

public slots:
    void startCountdown();
    void stopCountdown();
    void resetTimer();

private slots:
    void updateTimer();

private:
    QTimer *timer;
    int remainingTime;
};

#endif // COUNTDOWNTIMER_H
