#ifndef TIMEMEASURE_H
#define TIMEMEASURE_H
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class TimeWriter {
    public:
    string pathToFile;
    string identifire;
    chrono::_V2::high_resolution_clock::time_point startTime;
    chrono::_V2::high_resolution_clock::time_point stopTime;
    void start();
    void stop();
    void writeTime();
    double getTime();
    TimeWriter(string path, string identifire);
    TimeWriter();
};
#endif
