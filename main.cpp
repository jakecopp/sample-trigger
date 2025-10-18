#include <iostream>
#include <cmath>
#include "AudioFile.h"
#include "sample_trigger.h"

using namespace std;

int main(int argc, char* argv[]) {

    ///////////////// ARGUMENTS ////////////////
    string filename = "";
    string samplename = "";
    

    int a = 1;
    while (a < argc) {
        if (((string)argv[a]).compare("--filename") == 0) {
            a++;
            filename = argv[a];
        } else if (((string)argv[a]).compare("--sample") == 0) {
            a++;
            samplename = argv[a];
        } else {
            cerr << "Invalid arguments!" << endl;
            return -1;
        }
        a++;
    }
    if (filename.compare("") == 0) {
        cerr << "No file given!" << endl;
        return -1;
    }
    


    ///////////////// INITIALIZATION ////////////////
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

    int millisecondsCooldown = 50;
    double threshold = 0.05;
    
    processTrigger(millisecondsCooldown, threshold, infile, samplefile, "output-audio-file.wav");

}

/*


TODO:
detect trasient, first in sample, then in source



applySample method : method that applies the sample to the output track in one go in
    regards to where the transient is



basic transient detection vs advanced (first decrease above average?)

Find first transient:

max = 0
localmax = 0

while cur samples -> 0 to n-2
    do while sign is the same for cur, cur+1
        if abs(cur) > local max
            localmax = cur
    if localmax > max
        max = localmax
        localmax = 0
    else break

*/