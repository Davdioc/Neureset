#include "eegwaveformgenerator.h"
#include <cmath> // for sine wave generation and other math functions

EEGWaveformGenerator::EEGWaveformGenerator(QWidget *parent)
    : QWidget(parent), customPlot(new QCustomPlot(this)) {
    setupPlot();
}

EEGWaveformGenerator::~EEGWaveformGenerator() {
}

void EEGWaveformGenerator::updatePlot(const QVector<double>& waveformData) {
    QVector<QCPGraphData> graphData(waveformData.size());

    for (int i = 0; i < waveformData.size(); ++i) {
        graphData[i].key = i; // x-coordinate
        graphData[i].value = waveformData[i]; // y-coordinate
    }

    if (customPlot->graph(0)) { // Check if the graph exists
        customPlot->graph(0)->data()->set(graphData);
        customPlot->replot();
    } else {

    }
}

//resets the plot to be empty
void EEGWaveformGenerator::resetPlot() {
    if (customPlot->graph(0)) {
        customPlot->graph(0)->data()->clear();
        customPlot->replot();
    } else {
        qDebug() << "No graph to clear.";
    }
}



void EEGWaveformGenerator::setupPlot() {
    // Basic plot setup (axis labels, ranges, etc.)
    customPlot->addGraph();
    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("Amplitude");
    customPlot->xAxis->setRange(0, 100);
    customPlot->yAxis->setRange(-1, 1);
    customPlot->resize(this->size());           // Resize plot to take the full area of the widget

}

void EEGWaveformGenerator::resizeEvent(QResizeEvent *event) {
    customPlot->resize(event->size());
    QWidget::resizeEvent(event);
}
