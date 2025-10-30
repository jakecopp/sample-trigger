#include "sample_trigger.h"

using namespace std;

int detectTransient(AudioFile<double>& samplefile) {
    double max = 0;
    int transIndex = 0;
    for (int channel = 0; channel < samplefile.getNumChannels(); channel++) {
        for (int i = 0; i < samplefile.getNumSamplesPerChannel(); i++) {
            double currentSample = samplefile.samples[channel][i]; 
            if (abs(currentSample) > max) {
                max = abs(currentSample);
                transIndex = i;
            }
        }
    }
    return transIndex;
}

int detectTransient(AudioFile<double>& file, int start, int end) {
    double max = 0;
    int transIndex = 0;
    for (int channel = 0; channel < file.getNumChannels(); channel++) {
        for (int i = start; i < end; i++) {
            double currentSample = file.samples[channel][i]; 
            if (abs(currentSample) > max) {
                max = abs(currentSample);
                transIndex = i;
            }
        }
    }
    return transIndex;
}


int detectFirstTransient(AudioFile<double>& file) {
    double max = 0;
    int maxIndex = 0;
    double localmax = 0;
    int localmaxIndex = 0;
    // double prevLocalmax = -1;
    int channel = 0;

    double averageAmplitude = getAverageAmplitude(file);

    int i = 0;
    while (i < file.getNumSamplesPerChannel() - 1) {
        while (nextSampleCrossesZero(i, channel, file)) {
            double currentSample = file.samples[0][i]; 

            if (abs(currentSample) > localmax) {
                localmax = abs(currentSample);
                localmaxIndex = i;
            } 
            i++;
        }

        if (localmax > max) {
            max = localmax;
            maxIndex = localmaxIndex;
            localmax = 0;
        } else {
            if (max >= averageAmplitude) {   // detect first decreasing peak above the average 
                break;
            }
        }
        i++;
    }

    return maxIndex;
}

bool isSameSign(double a, double b) {
    return ((a < 0) == (b < 0));  
}

void applySample(int index, int transientOffset, AudioFile<double>& samplefile, AudioFile<float>& outfile) {
    int startIndex = index - transientOffset;
    for (int i = 0; i < samplefile.getNumSamplesPerChannel(); i++) {
        for (int channel = 0; channel < samplefile.getNumChannels(); channel++) {
            if (startIndex + i >= 0 && startIndex + i < outfile.getNumSamplesPerChannel()) {
                outfile.samples[channel][startIndex + i] = outfile.samples[channel][startIndex + i] + samplefile.samples[channel][i]; 
            }   
        }
    }
}

void applySampleWithVelocity(int index, int transientOffset, AudioFile<double>& samplefile, AudioFile<float>& outfile, double peakValue) {
    int startIndex = index - transientOffset;
    double t = samplefile.samples[0][transientOffset];  //only works for mono


    for (int i = 0; i < samplefile.getNumSamplesPerChannel(); i++) {
        for (int channel = 0; channel < samplefile.getNumChannels(); channel++) {
            if (startIndex + i >= 0 && startIndex + i < outfile.getNumSamplesPerChannel()) {
                double outValue = (samplefile.samples[channel][i] / t) * peakValue;
                outfile.samples[channel][startIndex + i] = outfile.samples[channel][startIndex + i] + outValue; 
            }   
        }
    }
}

void processTrigger(int millisecondsCooldown, double threshold, bool useVelocity, AudioFile<double>& infile, AudioFile<double>& samplefile, string outputFileName) {
    // Convert release cooldown time from ms to samples using sample rate 
    int samplesCooldown = (int) (((double)infile.getSampleRate() / 1000.0) * (double) millisecondsCooldown);
    // Prepare output 
    AudioFile<float> outfile;
    outfile.setNumChannels (samplefile.getNumChannels()); // outfile and sample should have same number of channels
    outfile.setNumSamplesPerChannel (infile.getNumSamplesPerChannel());
    // Detect transient
    int sampleTransientOffset = detectTransient(samplefile);
    // cout << transientOffset << endl;
    // Apply samples
    int cooldown = 0;
    int triggerCount = 0;
    double curPeak = 0.0;
    for (int channel = 0; channel < infile.getNumChannels(); channel++) {
        for (int i = 0; i < infile.getNumSamplesPerChannel(); i++) {
            double currentSample = infile.samples[channel][i];  // detects only mono input or left channel only
            if (currentSample > curPeak) {
                curPeak = currentSample;
            }
            if (currentSample >= threshold && cooldown == 0) {
                int sourceTransientOffset = detectTransient(infile, i, i + samplesCooldown);

                if (useVelocity) {
                    applySampleWithVelocity(sourceTransientOffset, sampleTransientOffset, samplefile, outfile, infile.samples[channel][sourceTransientOffset]);
                } else {
                    applySample(sourceTransientOffset, sampleTransientOffset, samplefile, outfile);
                }

                triggerCount++;
                cooldown = samplesCooldown;
            } else {
                if (cooldown > 0) {
                    cooldown--;
                }
            }
            if (nextSampleCrossesZero(i, channel, infile)) {
                curPeak = 0;
            }
        }
        if (triggerCount > 0) {
            break;
        }
    }

    // // Save output as
    string outputFilePath = outputFileName; 
    outfile.save (outputFilePath, AudioFileFormat::Wave);
    // // Report
    // cout << triggerCount << " samples triggered accross " << infile.getNumChannels() << " channel(s)." << endl;
    // cout << "File: " << outputFilePath << endl;
    // outfile.printSummary();

}

int triggerSamples(int millisecondsCooldown, double threshold, bool useVelocity, std::string filename, std::string samplename, std::string outfilename) {
    // Input File
    AudioFile<double> infile;
    if (!infile.load (filename)) {
        cerr << "File not found!" << endl;
        return -1;
    }
    // cout << "File: " << filename << endl;
    // infile.printSummary();
    // Sample File
    AudioFile<double> samplefile;
    if (!samplefile.load (samplename)) {
        cerr << "File not found!" << endl;
        return -1;
    }
    // cout << "File: " << samplename << endl;
    // samplefile.printSummary();
    processTrigger(millisecondsCooldown, threshold, useVelocity, infile, samplefile, outfilename);
    return 0;
}

double decibelsFromGain(double gain) {
    return std::log10 (gain) * 20.0;
}


bool nextSampleCrossesZero(int index, int channel, AudioFile<double>& file) {
    return (index < file.getNumSamplesPerChannel() - 1) && isSameSign(file.samples[channel][index], file.samples[channel][index + 1]);
}

double getAverageAmplitude(AudioFile<double>& file) {
    double sum = 0;
    for (int channel = 0; channel < file.getNumChannels(); channel++) {
        for (int i = 0; i < file.getNumSamplesPerChannel(); i++) {
            double currentSample = file.samples[channel][i]; 
            if (currentSample > 0)
                sum += abs(currentSample);
        }
    }

    return sum / (double) (file.getNumChannels() * file.getNumSamplesPerChannel());
}