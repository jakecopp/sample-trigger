#include "AudioFile.h"


int detectTransient(AudioFile<double>& samplefile);
void applySample(int index, int channel, AudioFile<double>& samplefile, AudioFile<float>& outfile);
