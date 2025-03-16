#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QRandomGenerator>
#include <QVector>

class electrode
{

public:
    electrode(int id);              //constructor

    int getId() const;              //returns the id

    double getFrequency() const;    //returns the frequency
    void generateFrequency();       //generates a frequency and sets it
    QVector<double> getWaveform() const;

private:
    int id;
    double frequency;
    double f1, f2, f3, f4;                      // Frequencies of the sine waves
    double A1, A2, A3, A4;                      // Amplitudes of the sine waves
    double calculateDominantFrequency();
    QVector<double> waveform;
    QVector<double> generateWaveform(double f1, double A1, double f2, double A2, double f3, double A3, double f4, double A4);
};

#endif // ELECTRODE_H
