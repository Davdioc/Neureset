#ifndef EEGWAVEFORMGENERATOR_H
#define EEGWAVEFORMGENERATOR_H

#include <QWidget>
#include "qcustomplot.h"

class EEGWaveformGenerator : public QWidget {
    Q_OBJECT

public:
    explicit EEGWaveformGenerator(QWidget *parent = nullptr);
    ~EEGWaveformGenerator();

    // Updates the plot for a selected electrode
    void updatePlot(const QVector<double>& waveformData);
    void resetPlot();

private:
    QCustomPlot *customPlot; // The QCustomPlot widget
    QVector<QVector<double>> waveforms; // Stores waveforms for each electrode
    int numElectrodes = 7; // Number of electrodes

    // Helper method to setup the plot
    void setupPlot();
protected:
     void resizeEvent(QResizeEvent *event) override;
};

#endif // EEGWAVEFORMGENERATOR_H
