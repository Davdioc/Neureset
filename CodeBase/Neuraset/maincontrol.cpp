#include "maincontrol.h"

MainControl::MainControl(QObject *parent) : QObject(parent) {
    batteryValue = 100;

    dbManager = new DatabaseManager();

    mainUI = new MainUI(nullptr, dbManager);
    pcUi = new pcUI(nullptr, dbManager);

    devPanel = new DevPanel(mainUI, pcUi);

    lenstest = new class lensTest(nullptr);

    pcUi->resize(500,500);
    sessionPaused = false;
    headsetContact = false;

    // connecting the EEGWaveformGenerator from lensTest to the MainUI
    mainUI->setWaveformGenerator(lenstest->getWaveformGenerator());

    connectSlots();
    togglePower();
    mainUI->toggleLostContactLight();

    mainUI->show();
    devPanel->show();

}

MainControl::~MainControl() {
    delete mainUI;
    delete pcUi;
    delete lenstest;
    delete dbManager;
}

void MainControl::connectSlots(){

    QThread *timerThread = new QThread(this);
    countdownTimer = new CountdownTimer(32);
    countdownTimer->moveToThread(timerThread);

    connect(mainUI, &MainUI::playButtonClicked, this, &MainControl::playButtonPressed);
    connect(mainUI, &MainUI::pauseButtonClicked, this, &MainControl::pauseButtonPressed);
    connect(mainUI, &MainUI::stopButtonClicked, this, &MainControl::endSession);
    connect(mainUI, &MainUI::powerButtonClicked, this, &MainControl::powerButtonPressed);
    connect(mainUI, &MainUI::menuButtonClicked, this, &MainControl::menuButtonPressed);
    connect(mainUI, &MainUI::timeButtonClicked, this, &MainControl::timeButtonPressed);
    connect(countdownTimer, &CountdownTimer::countdownUpdated, this, &MainControl::updateCountdownDisplay);
    connect(this, &MainControl::startTimerSignal, countdownTimer, &CountdownTimer::startCountdown, Qt::QueuedConnection);
    connect(this, &MainControl::stopTimerSignal, countdownTimer, &CountdownTimer::stopCountdown, Qt::QueuedConnection);
    connect(this, &MainControl::resetTimerSignal, countdownTimer, &CountdownTimer::resetTimer, Qt::QueuedConnection);
    connect(lenstest, &lensTest::sessionEnded, this, &MainControl::endSession);
    connect(timerThread, &QThread::finished, countdownTimer, &QObject::deleteLater);
    connect(timerThread, &QThread::finished, timerThread, &QObject::deleteLater);
    connect(lenstest, &lensTest::electrodeSignal, this, &MainControl::flashTreatmentLight);
    connect(lenstest, &lensTest::logMessage, this, &MainControl::acceptLog);
    connect(lenstest, &lensTest::sessionComplete, this, &MainControl::logSessionData);
    connect(devPanel, &DevPanel::powerResetButtonPressed, this, &MainControl::resetBattery);
    connect(devPanel, &DevPanel::headsetButtonPressed, this, &MainControl::toggleContact);

    timerThread->start();
}



void MainControl::playButtonPressed() {
    if (!headsetContact) {
        qDebug() << "Headset contact not detected. Please adjust the headset.";
        return;
    }
    if (batteryValue < 32 && deviceOn) {
        qDebug() << "Device Low Battery! Please Recharge";
        return;
    }
    if (!deviceOn || !mainUI->isReadyForSession()) {
        qDebug() << "Device is off or not ready. Ignoring play button press.";
        return;
    }

    if (sessionPaused && lenstest->isRunning) {
        emit startTimerSignal(); // Resume the timer
        lenstest->togglePause(); // Ensure togglePause only changes state if paused
        sessionPaused = false;
    } else if (!countdownTimer->isRunning() && !lenstest->isRunning) {
        mainUI->setTreatmentBar(0);
        emit startTimerSignal();
        lenstest->startSession();
        sessionPaused = false;
    } else {
        qDebug() << "Timer is already running. Ignoring play button press.";
    }
}


void MainControl::pauseButtonPressed() {
    if (!deviceOn || !mainUI->isReadyForSession()) {
        qDebug() << "Device is off or not ready. Ignoring pause button press.";
        return;
    }
    if (countdownTimer->isRunning() && lenstest->isRunning && !sessionPaused) {
        emit stopTimerSignal();             // Pause the timer
        lenstest->togglePause();        // Toggle the pause state
        sessionPaused = true;
    } else {
        qDebug() << "Program is not running or already paused.";
    }
}

void MainControl::endSession(){
     if(deviceOn && lenstest && lenstest->isRunning){
         if(lenstest->isPaused){
             lenstest->togglePause();
         }
         lenstest->stopTreatment();
         lenstest->resetGraph();
         emit stopTimerSignal();
         emit resetTimerSignal();
         sessionPaused = false;
         mainUI->setTimeLCD("00:00");
         mainUI->setTreatmentBar(0);
         if(mainUI->treatmentLightLit()){
             qDebug() << "resetting the treatment light";
             mainUI->toggleTreatmentLight();
         }
     }
}

void MainControl::powerButtonPressed(){
    if(batteryValue < 5){
        qDebug("Power Low, please recharge!");
    }else{
    qDebug() << "Power Button Pressed!";
    togglePower();
    }
}

void MainControl::togglePower(){
    endSession();
    deviceOn = !deviceOn;
    if(deviceOn){
        mainUI->powerOn();             //this will later have to access some saved power value
    }else{
        mainUI->powerOff();
    }
}

void MainControl::menuButtonPressed(){
    if(deviceOn){
        qDebug() << "Menu Button Pressed!";
    }
}

void MainControl::logSessionData(){
    if(!setDateTime.isValid()){
        setDateTime = QDateTime::currentDateTime();
    }
    dbManager->addSessionData(setDateTime, currentLog);                        //saves info with date and time in database

    pcUi->refreshData();
}

void MainControl::updateCountdownDisplay(int secondsLeft) {
    std::string timeString = lenstest->timer.formatTime(secondsLeft);       //convert seconds to the correct format
    mainUI->setTimeLCD(timeString);                                         //set the LCD to the formatted time
    batteryValue -= 1;
    if(mainUI->getTreatmentProgress() == 0){
        mainUI->setTreatmentBar(mainUI->getTreatmentProgress()+ 7);
    }else {
    mainUI->setTreatmentBar(mainUI->getTreatmentProgress()+ 3);
    }
    mainUI->setChargeBar(batteryValue);
}

void MainControl::resetBattery(){
    batteryValue = 100;
    mainUI->setChargeBar(batteryValue);
}

void MainControl::flashTreatmentLight(){
    mainUI->toggleTreatmentLight();
}

void MainControl::toggleContact(){
    if(deviceOn){
    //stop all processes occuring first
    pauseButtonPressed();
    //then toggle the contact light
    if(mainUI->contactLightLit()){
        mainUI->toggleContactLight();
        mainUI->toggleLostContactLight();
        headsetContact = false;
    }else{
        mainUI->toggleLostContactLight();
        mainUI->toggleContactLight();
        headsetContact = true;
    }
}
}
void MainControl::acceptLog(QString message){
    currentLog += "\n" + message;
}

void MainControl::timeButtonPressed(){
    setDateTime = mainUI->getDateTimeInput();
}
