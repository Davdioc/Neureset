#include "lensTest.h"


//initializing the electrode
lensTest::lensTest(QObject *parent)
    : QObject{parent}, baselineFrequency(0.0), currentFrequency(0.0), signalStrength(0.0)
{
    for(int i = 0; i < numElectrodes; i++){
        electrodes.append(new electrode(i));
    }
    waveformGenerator = new EEGWaveformGenerator(nullptr);
}

lensTest::~lensTest(){
    for (electrode* electrode : electrodes) {
           delete electrode; // Proper memory cleanup
       }
}

void lensTest::startSession(){
    if(!isPaused && !isRunning){
    //toggle isRunning
        isRunning = true;
    //start by resetting electrode states
    reset();

    //administers the treatment
    adminsterTreatment();
    }else if (isPaused){
        togglePause();
    }

}

//takes one minute to calculate the baseline, interating over all the nodes every 5 seconds
double lensTest::calculateBaseline() {
    const int samplesPerMinute = 60;
    double totalFrequency = 0;
    double maxAverageFrequency = 0;
    QVector<double> bestWaveform;                           // this is to chose the best waveform to save

    for(electrode* electrode : electrodes) {
        double totalElectrodeFrequency = 0;

        for (int i = 0; i < samplesPerMinute; ++i) {
            electrode->generateFrequency();                 //generates a waveform and frequency for the electrode
            totalElectrodeFrequency += electrode->getFrequency();
            timer.waitMilliseconds(6);                      // simulating a delay of 5 ms x 60 x 7
        }

        double averageFrequency = totalElectrodeFrequency / samplesPerMinute;
        totalFrequency += averageFrequency;

        if (averageFrequency > maxAverageFrequency) {
            maxAverageFrequency = averageFrequency;
            bestWaveform = electrode->getWaveform();
        }
    }

    waveformGenerator->updatePlot(bestWaveform);

    return totalFrequency / numElectrodes;
}


void lensTest::adminsterTreatment(){
    const int treatmentDuration = 1000;                // Treatment time in milliseconds (1 second)
    const int updateInterval = treatmentDuration / 16; // Updates every 1/16 of a second

    qDebug() << "Starting treatment for all electrodes.";
    bool sessionEndedEarly = false;

    for (int i = 0; i < electrodes.size(); ++i) { // Goes through each electrode
        if(sessionEndedEarly || stopRequested){
            break;                                // Stops processing electrodes
        }

        baselineFrequency = calculateBaseline();
        double currentFrequency = baselineFrequency; // Uses the shared baseline frequency
        qDebug() << "Electrode" << i + 1 << ": Starting treatment with baseline frequency" << baselineFrequency << "Hz";

        //logging Code for the database
        QString startMsg = QString("Pre - Electrode %1: %2 Hz")
                                    .arg(i + 1)
                                    .arg(baselineFrequency, 0, 'f', 3); // Format frequency with 3 decimal places
        emit logMessage(startMsg);


        emit electrodeSignal();     //toggles the light

        for (int elapsed = 0; elapsed < treatmentDuration; elapsed += updateInterval) {
            if (isPaused) {
                int pauseTimeCount = 0;
                while (isPaused && pauseTimeCount < 10) {
                    timer.waitSeconds(1);           //waits 1 second
                    pauseTimeCount++;
                }
                if (pauseTimeCount >= 10) {
                    sessionEndedEarly = true;
                    emit sessionEnded();
                    break;                         // Exit the loop if paused for 5 minutes
                }
            }

            // Treatment logic
            currentFrequency += offsetFrequency;
            qDebug() << "Electrode" << i + 1 << ": Frequency updated to" << currentFrequency << "Hz";

            timer.waitMilliseconds(updateInterval);
            electrodes[i]->generateFrequency();                         //finds the new frequency
            currentFrequency = electrodes[i]->getFrequency();
        }

        emit electrodeSignal();                   //toggles the light


        if (!sessionEndedEarly) {
            qDebug() << "Electrode" << i + 1 << ": Treatment completed with final frequency" << currentFrequency << "Hz";

            //logging code to mainControl
            QString endMsg = QString("Post - Electrode %1: %2 Hz")
                                      .arg(i + 1)
                                      .arg(currentFrequency, 0, 'f', 3);
           emit logMessage(endMsg);
        } else {
            break; // If the session ended early, no need to continue with other electrodes
        }
    }

    if (sessionEndedEarly || stopRequested) {
        qDebug() << "Session ended early";
        isRunning = false;
    } else {
        qDebug() << "Treatment completed for all electrodes.";
        emit sessionComplete();
        resetGraph();
        isRunning = false;
    }
}


void lensTest::togglePause(){
    if(isRunning){
    isPaused = !isPaused;
    if(isPaused){
        qDebug() << "Session Paused";
    }else{
        qDebug() << "Session resumed";
    }
    }
}

EEGWaveformGenerator* lensTest::getWaveformGenerator() const{
    return waveformGenerator;
}

void lensTest::resetGraph(){
    waveformGenerator->resetPlot();
}

void lensTest::reset(){
     qDebug() << "Resetting the treatment session.";
    //resets the data in electrode control
        baselineFrequency = 0.0;
        currentFrequency = 0.0;
        signalStrength = 0.0;
        stopRequested = false;

    qDebug() << "All session data has been reset.";
}




