#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "defs.h"
#include "mainUI.h"
#include "pcui.h"
#include "lensTest.h"
#include "timer.h"
#include "countdowntimer.h"
#include "databasemanager.h"
#include "devpanel.h"


class MainControl : public QObject
{
    Q_OBJECT
public:
    explicit MainControl(QObject *parent = nullptr);
    ~MainControl();

signals:
    void startTimerSignal();
    void stopTimerSignal();
    void resetTimerSignal();


private:

    MainUI* mainUI;
    pcUI* pcUi;
    DevPanel* devPanel;
    lensTest* lenstest;
    DatabaseManager* dbManager;

    int batteryValue;

    void connectSlots();
    void updateCountdownDisplay(int secondsLeft);
    void togglePower();

    CountdownTimer* countdownTimer;

    QString currentLog;
    QDateTime setDateTime;

    bool sessionPaused;
    bool headsetContact;
    bool deviceOn = true;

public slots:
    void playButtonPressed();
    void pauseButtonPressed();
    void endSession();
    void powerButtonPressed();
    void menuButtonPressed();
    void timeButtonPressed();
    void resetBattery();
    void flashTreatmentLight();
    void toggleContact();
    void logSessionData();
    void acceptLog(QString message);
};

#endif // MAINCONTROL_H
