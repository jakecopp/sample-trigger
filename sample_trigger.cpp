#include "sample_trigger.h"

using namespace std;

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
            outfile.samples[channel][startIndex + i] = outfile.samples[channel][startIndex + i] + samplefile.samples[channel % samplefile.getNumChannels()][i]; // 0's for mono, can be upgraded to multichannel
        }   
    }
}



void processTrigger(int millisecondsCooldown, double threshold, AudioFile<double>& infile, AudioFile<double>& samplefile, string outputFileName) {
    // Convert release cooldown time from ms to samples using sample rate 
    int samplesCooldown = (int) (((double)infile.getSampleRate() / 1000.0) * (double) millisecondsCooldown);
    // Prepare output 
    AudioFile<float> outfile;
    outfile.setNumChannels (infile.getNumChannels());
    outfile.setNumSamplesPerChannel (infile.getNumSamplesPerChannel());
    // Apply samples
    int cooldown = 0;
    int triggerCount = 0;
    for (int channel = 0; channel < infile.getNumChannels(); channel++) {
        for (int i = 0; i < infile.getNumSamplesPerChannel(); i++) {
            double currentSample = infile.samples[channel][i]; 
            if (currentSample >= threshold && cooldown == 0) {
                applySample(i, channel, samplefile, outfile);
                triggerCount++;
                cooldown = samplesCooldown;
            } else {
                if (cooldown > 0) {
                    cooldown--;
                }
            }
        }
    }
    // Save output as
    string outputFilePath = outputFileName; 
    outfile.save (outputFilePath, AudioFileFormat::Wave);
    // Report
    cout << triggerCount << " samples triggered accross " << infile.getNumChannels() << " channel(s)." << endl;
    cout << "File: " << outputFilePath << endl;
    outfile.printSummary();

}
