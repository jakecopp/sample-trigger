#include "AudioFile.h"

int detectTransient(AudioFile<double>& samplefile);

int detectFirstTransient(AudioFile<double>& samplefile);

bool isSameSign(double a, double b);

double getAverageAmplitude(AudioFile<double>& file);

void applySample(int index, int channel, int transientOffset, AudioFile<double>& samplefile, AudioFile<float>& outfile);

void processTrigger(int millisecondsCooldown, double threshold, AudioFile<double>& infile, AudioFile<double>& samplefile, std::string outputFileName);

int triggerSamples(int millisecondsCooldown, double threshold, std::string filename, std::string samplename, std::string outfilename);