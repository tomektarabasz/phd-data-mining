#include <iostream>
#include "models/purity.h"
#include "helpers/timeMeasure.h"
#include "preprocessing/mdPointLoader.h"
#include "./preprocessing/dataWriter.h"
#include "helpers/identyficator.h"
#include <vector>
#include <map>
#include "models/mdPoint.h"
#include "helpers/findNeighbours.h"
#include <algorithm>

using namespace std;

void filterNeighbourFromAlreadyCalculatedPoints(vector<unsigned long> &seed, vector<MDPoint> &allData)
{
    vector<unsigned long> result;
    copy_if(seed.begin(), seed.end(), back_inserter(result), [&allData](unsigned long index)
            { return allData[index].clasterId == -1; });
    seed = result;
}

void buildClaster(vector<unsigned long> &seeds, vector<MDPoint> &allData, int numberOfMinNeighbour, long clasterId, double eps)
{
    if (seeds.size() < 1)
    {
        return;
    }
    for (auto index : seeds)
    {
        MDPoint &p = allData[index];
        if (p.clasterId == -1)
        {
            p.clasterId = clasterId;
        }
    }
    MDPoint &point = allData[seeds.back()];

    findNeigbour(point, allData, eps);
    if (point.neighbourIndexes.size() >= numberOfMinNeighbour)
    {
        vector<unsigned long> seeds = point.neighbourIndexes;
        filterNeighbourFromAlreadyCalculatedPoints(seeds, allData);
        // point.assingToTheSameClusterId(allData);
        buildClaster(seeds, allData, numberOfMinNeighbour, clasterId, eps);
    };
    seeds.pop_back();
    buildClaster(seeds, allData, numberOfMinNeighbour, clasterId, eps);
}

int main()
{
    string pathToStoreTimeOfExecution = "Data/time.csv";

    //Read data
    string pathToFile = "Data/lecture.csv";
    // string pathToFile = "Data/points.csv";
    // string pathToFile = "Data/dim512.csv";
    // string pathToFile = "Data/complex9.csv";
    // string pathToFile = "Data/cluto-t7-10k.csv";
    // string pathToFile = "Data/letter.csv";
    // string pathToFile = "Data/points.csv";
    // end choose of files
    MDPointLoader dataReader;
    vector<MDPoint> *data = new vector<MDPoint>(dataReader.getData(pathToFile));
    vector<MDPoint> &dataR = *data;
    // Main process
    TimeWriter *timeWriter = new TimeWriter(pathToStoreTimeOfExecution, Identyficator("dbscan", to_string(dataR.size())));
    timeWriter->start();
    int numberOfMinNeighbour = 4;
    double eps = 2;
    unsigned long currentClasterId = 1;
    int currentNeighboursNumber;

    for (auto &currentPoint : dataR)
    {
        if (currentPoint.clasterId == -1)
        {
            findNeigbour(currentPoint, dataR, eps);
            currentNeighboursNumber = currentPoint.neighbourIndexes.size();
            // if it it core assing cluster id
            if (currentNeighboursNumber >= numberOfMinNeighbour)
            {
                currentPoint.clasterId = currentClasterId;
                // currentPoint.assingToTheSameClusterId(dataR);
                buildClaster(currentPoint.neighbourIndexes, dataR, numberOfMinNeighbour, currentClasterId, eps);
                currentClasterId++;
            }
        }
    }
    timeWriter->stop();
    timeWriter->writeTime();

    DataWriter dataWriter;
    string pathToResultFile = "Data/dbscanResult.csv";
    dataWriter.writeClasteringResults(pathToResultFile, dataR);
    delete data;

    return 0;
};
