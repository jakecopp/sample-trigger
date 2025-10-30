#include "AudioFile.h"

int detectTransient(AudioFile<double>& samplefile);

int detectTransient(AudioFile<double>& samplefile, int start, int end);

int detectFirstTransient(AudioFile<double>& samplefile);

bool isSameSign(double a, double b);

double getAverageAmplitude(AudioFile<double>& file);

void applySample(int index, int transientOffset, AudioFile<double>& samplefile, AudioFile<float>& outfile);

void applySampleWithVelocity(int index, int transientOffset, AudioFile<double>& samplefile, AudioFile<float>& outfile, double peakValue);

void processTrigger(int millisecondsCooldown, double threshold, bool useVelocity, AudioFile<double>& infile, AudioFile<double>& samplefile, std::string outputFileName);

int triggerSamples(int millisecondsCooldown, double threshold, bool useVelocity, std::string filename, std::string samplename, std::string outfilename);

double decibelsFromGain(double gain);

bool nextSampleCrossesZero(int index, int channel, AudioFile<double>& file);