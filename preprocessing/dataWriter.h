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
    string algorithm;
    string pathToFile;
    string nameOfInputFile;
    int numberOfPoints;
    int numberofDismensions;
    string params;
    double readingDatasetTime;
    double sortingDataTime;
    double findingNeighboursAndReverNeighbourTime;
    double clasteringTime;
    double totalRuntime;
    int numberOfClusters;
    int numberOfCorePoints;
    int numberOfBorderPoints;
    int numberOfNoisePoint;
    int avarOfDistanceCalculations;

    void writeLine(string);
    STAT(string pathToFile);
    void gatherDataFromCollection(vector<MDPoint>& data);
    void writeSTATFile();
};
#endif