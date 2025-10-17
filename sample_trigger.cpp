#include "sample_trigger.h"

int detectTransient(AudioFile<double>& samplefile) {
    double max = 0;
    int transIndex = 0;
    for (int i = 0; i < samplefile.getNumSamplesPerChannel(); i++) {
        double currentSample = samplefile.samples[0][i]; 

        if (abs(currentSample) > max) {
            max = abs(currentSample);
            transIndex = i;
        }
    }
    return transIndex;
}
void applySample(int index, int channel, AudioFile<double>& samplefile, AudioFile<float>& outfile) {
    int startIndex = index - detectTransient(samplefile);
    for (int i = 0; i < samplefile.getNumSamplesPerChannel(); i++) {
        if (startIndex + i >= 0 && startIndex + i < outfile.getNumSamplesPerChannel()) {
            // for (int channel = 0; channel < outfile.getNumChannels(); channel++) {
                outfile.samples[channel][startIndex + i] = outfile.samples[channel][startIndex + i] + samplefile.samples[channel % samplefile.getNumChannels()][i]; // 0's for mono, can be upgraded to multichannel
            // }
        }
        
        
    }
}