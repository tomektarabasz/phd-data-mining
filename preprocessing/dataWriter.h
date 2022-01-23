#ifndef DATAWRITER_H
#define DATAWRITER_H
#include "../models/point.h"
#include "../models/mdPoint.h"
#include <vector>
#include <string>

using namespace std;

class DataWriter
{
public:
    static void write(string pathToFile, vector<Point>& data);
    static void writeMDPoints(string pathToFile, vector<MDPoint>& data);
    static void writeClasteringResults(string pathToFile, vector<MDPoint> &data);
    static void OUT();
    ~DataWriter();
};

class STAT {
    public:
    string pathToFile;
    void writeLine(string);
    STAT(string pathToFile);
};
#endif