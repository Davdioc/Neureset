#include "electrode.h"

electrode::electrode(int id)
    : id(id), frequency(0.0)
{
}

int electrode::getId() const{
    return id;
}

double electrode::getFrequency() const {
    return frequency;
}

QVector<double> electrode::getWaveform() const {
    return waveform;
}


void electrode::generateFrequency() {
    // randomly generate frequencies within EEG frequency bands
    // alpha [8-12Hz], Beta [12-30Hz], Delta [1-4Hz], Theta [4-8Hz], Gamma [25-140Hz]
    QRandomGenerator *generator = QRandomGenerator::global();

    // assigning random frequencies
    f1 = generator->bounded(8, 13); // Alpha band
    f2 = generator->bounded(12, 31); // Beta band
    f3 = generator->bounded(25, 141); // Gamma band
    f4 = generator->bounded(1,5);      //Delta band

    // assigning random amplitudes using generateDouble
    A1 = 0.5 + generator->generateDouble() * (1.5 - 0.5);
    A2 = 0.5 + generator->generateDouble() * (1.5 - 0.5);
    A3 = 0.5 + generator->generateDouble() * (1.5 - 0.5);
    A4 = 0.5 + generator->generateDouble() * (1.5 - 0.5);

    // generate the waveform
    waveform = generateWaveform(f1, A1, f2, A2, f3, A3, f4, A4);

    // calculate the dominant frequency
    frequency = calculateDominantFrequency();
}

QVector<double> electrode::generateWaveform(double f1, double A1, double f2, double A2, double f3, double A3, double f4, double A4) {
    const int sampleCount = 100;
    QVector<double> waveform(sampleCount);
    double maxAmplitude = 0.0;

    // Generate the waveform based on the given frequencies and amplitudes
    for (int i = 0; i < sampleCount; ++i) {
        waveform[i] = A1 * sin(2 * M_PI * f1 * i / sampleCount) +
                      A2 * sin(2 * M_PI * f2 * i / sampleCount) +
                      A4 * sin(2 * M_PI * f4 * i / sampleCount) +
                      A3 * sin(2 * M_PI * f3 * i / sampleCount);
        // updates the maximum amplitude
        if (fabs(waveform[i]) > maxAmplitude) {
            maxAmplitude = fabs(waveform[i]);
        }
    }

    // prevents the waveform from exceeding 1
    if (maxAmplitude > 1) {
        for (double &amp : waveform) {
            amp /= maxAmplitude;
        }
    }

    return waveform;
}


double electrode::calculateDominantFrequency() {
    double fd = (f1 * A1 * A1 + f2 * A2 * A2 + f3 * A3 * A3 + f4 * A4 * A4) / (A1 * A1 + A2 * A2 + A3 * A3 + A4 * A4);
    return fd;
}
