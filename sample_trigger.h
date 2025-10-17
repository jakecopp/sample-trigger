#include "AudioFile.h"


int detectTransient(AudioFile<double>& samplefile);
void applySample(int index, int channel, AudioFile<double>& samplefile, AudioFile<float>& outfile);
void processTrigger(int millisecondsCooldown, double threshold, AudioFile<double>& infile, AudioFile<double>& samplefile, std::string outputFileName);
