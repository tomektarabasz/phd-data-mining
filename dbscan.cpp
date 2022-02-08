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
#include <sstream>

using namespace std;

bool isPointNeighborsEnoughToBeCorePoint(int neighbours, int minPoint)
{
    return neighbours >= minPoint;
}

void filterNeighbourFromAlreadyCalculatedPoints(vector<unsigned long> &seed, vector<MDPoint> &allData)
{
    vector<unsigned long> result;
    copy_if(seed.begin(), seed.end(), back_inserter(result), [&allData](unsigned long index)
            { return allData[index].clasterId == -1; });
    seed = result;
}

void buildClaster(vector<unsigned long> seeds, vector<MDPoint> &allData, int numberOfMinNeighbour, long clasterId, double eps)
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
    point.pointType = 0;
    if (point.neighbourIndexes.size() == 0)
    {
        findNeigbour(point, allData, eps);
    }

    if (isPointNeighborsEnoughToBeCorePoint(point.neighbourIndexes.size(), numberOfMinNeighbour))
    {
        point.pointType = 1;
        vector<unsigned long> seeds = point.neighbourIndexes;
        filterNeighbourFromAlreadyCalculatedPoints(seeds, allData);
        // point.assingToTheSameClusterId(allData);
        buildClaster(seeds, allData, numberOfMinNeighbour, clasterId, eps);
    };
    seeds.pop_back();
    buildClaster(seeds, allData, numberOfMinNeighbour, clasterId, eps);
}

int main(int argc, char **argv)
{
    string arg1 = argv[1];
    string arg2 = argv[2];
    string arg3 = argv[3]; // this is treated as tanimoto similarity
    cout << arg1 << endl;
    cout << arg2 << endl;
    cout << arg3 << endl;
    int fileNumber = stoi(arg1);
    int numberOfMinNeighbour = stoi(arg2);
    double epsTanimotoSimilarity = stod(arg3);
    double eps = 1 - epsTanimotoSimilarity;

    string pathToStoreTimeOfExecution = "Data/time.csv";
    string *paths = new string[5];
    paths[0] = "Data/lecture.csv";
    paths[1] = "Data/dim512.csv";
    paths[2] = "Data/complex9.csv";
    paths[3] = "Data/cluto-t7-10k.csv";
    paths[4] = "Data/letter.csv";

    string *fileIdentificators = new string[5];
    fileIdentificators[0] = "lecture.csv";
    fileIdentificators[1] = "dim512.csv";
    fileIdentificators[2] = "complex9.csv";
    fileIdentificators[3] = "cluto-t7-10k.csv";
    fileIdentificators[4] = "letter.csv";

    string postIndentificator = fileIdentificators[fileNumber];
    string pathToStoreSTATfile = "Data/STAT_dbscan_";
    ostringstream *oss = new ostringstream();
    *oss << pathToStoreSTATfile << postIndentificator;
    STAT statFile(oss->str());
    oss->str("");
    (*oss) << "Data/OUT_dbscan_" << postIndentificator;
    string pathToResultFile = oss->str();

    oss->str("");
    (*oss) << "Data/DEBUG_dbscan_" << postIndentificator;
    string pathToDEBUGFile = oss->str();
    delete oss;

    statFile.algorithm = "DBSCAN";

    //Read data
    // string pathToFile = "Data/lecture.csv";
    // string pathToFile = "Data/points.csv";
    // string pathToFile = "Data/dim512.csv";
    // string pathToFile = "Data/complex9.csv";
    // string pathToFile = "Data/cluto-t7-10k.csv";
    string pathToFile = paths[fileNumber];
    // end choose of files

    statFile.nameOfInputFile = pathToFile;

    TimeWriter totalTimer;
    TimeWriter timer;
    totalTimer.start();
    timer.start();
    MDPointLoader dataReader;
    vector<MDPoint> *data = new vector<MDPoint>(dataReader.getData(pathToFile));
    vector<MDPoint> &dataR = *data;
    timer.stop();
    statFile.readingDatasetTime = timer.getTime();
    // Main process
    TimeWriter *timeWriter = new TimeWriter(pathToStoreTimeOfExecution, Identyficator("dbscan", to_string(dataR.size())));
    timeWriter->start();

    // STAT params saving
    ostringstream ossParams;
    ossParams << "numberOfMinNeighbour = " << numberOfMinNeighbour << " eps = " << eps << " eps-tanimoto-similarity = " << epsTanimotoSimilarity;
    statFile.params = ossParams.str();
    // end STAT params saving

    unsigned long currentClasterId = 1;
    int currentNeighboursNumber;

    double findNeighbourTime = 0;
    double discoverClasterTime = 0;

    for (auto &currentPoint : dataR)
    {
        if (currentPoint.clasterId == -1)
        {
            findNeigbour(currentPoint, dataR, eps);
            currentNeighboursNumber = currentPoint.neighbourIndexes.size();
            // if it it core assing cluster id
            if (isPointNeighborsEnoughToBeCorePoint(currentNeighboursNumber, numberOfMinNeighbour))
            {
                currentPoint.clasterId = currentClasterId;
                currentPoint.pointType = 1;
                timer.start();
                buildClaster(currentPoint.neighbourIndexes, dataR, numberOfMinNeighbour, currentClasterId, eps);
                timer.stop();
                discoverClasterTime += timer.getTime();
                currentClasterId++;
            }
        }
    }
    totalTimer.stop();
    timeWriter->stop();
    timeWriter->writeTime();

    for (auto point : dataR)
    {
        findNeighbourTime += point.timeToFindNeighbour;
    }
    statFile.findingNeighboursAndReverNeighbourTime = findNeighbourTime;
    statFile.clasteringTime = discoverClasterTime;
    statFile.sortingDataTime = 0;
    statFile.totalRuntime = totalTimer.getTime();

    DataWriter dataWriter;
    dataWriter.writeClasteringResults(pathToResultFile, dataR);
    dataWriter.writeMDPoints(pathToDEBUGFile, dataR);
    statFile.gatherDataFromCollection(dataR);
    statFile.writeSTATFile();
    delete data;
    cout<< "finist";
    return 0;
};
