#ifndef LENSTEST_H
#define LENSTEST_H

#include <QObject>
#include <QVector>
#include <QThread>
#include <QDebug>
#include "lenstest.h"
#include "electrode.h"
#include "timer.h"
#include "eegwaveformgenerator.h"

class lensTest : public QObject
{
    Q_OBJECT

public:
    explicit lensTest(QObject *parent = nullptr);
    ~lensTest();

    void startSession();
    double calculateBaseline();
    void adminsterTreatment();
    void reset();
    void togglePause();
    void stopTreatment(){ stopRequested = true;}
    Timer timer;
    bool isRunning = false;
    EEGWaveformGenerator* getWaveformGenerator() const;
    void resetGraph();
    bool isPaused = false;

private:

    EEGWaveformGenerator* waveformGenerator;
    QVector<electrode*> electrodes; //an array of pointers to electrodes
    const int numElectrodes = 7;
    int id;
    double baselineFrequency;
    double currentFrequency;
    const double offsetFrequency = 5.0; //Hz
    double signalStrength;
    bool stopRequested = false;



signals:
    void sessionStarted();
    void sessionEnded();
    void electrodeSignal();
    void sessionComplete();
    QString logMessage(QString message);


};

#endif // LENSTEST_H
