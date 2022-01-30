#include "dataWriter.h"
#include <string>
#include <vector>
#include "../models/point.h"
#include <fstream>
#include <iostream>

using namespace std;

DataWriter::~DataWriter(){};

void DataWriter::write(string path, vector<Point> &data)
{
    ofstream resultFile(path);
    if (resultFile.is_open())
    {
        for (auto point : data)
        {
            resultFile << point.guid << "," << point.x1 << "," << point.x2 << "," << point.clasterId << "\n";
        }
        resultFile.close();
    }
    else
        cout << "Unable to open file";
};

void DataWriter::writeMDPoints(string pathToFile, vector<MDPoint> &data)
{
    ofstream resultFile(pathToFile);
    if (resultFile.is_open())
    {
        resultFile << "id,";
        int i = 0;
        for (auto attr : data[0].attributes)
        {
            resultFile << "x" << i << ",";
            i++;
        }
        resultFile << "len,";
        resultFile << "clasteId,";
        resultFile << "pointType,";
        resultFile << "timeToCalcNeighbours,";
        resultFile << "# distance calc,";
        resultFile << "len(rnnk),";
        resultFile << "kNN,";
        resultFile << "reverskNN,";
        resultFile << "NDF";
        resultFile << "\n";
        for (auto point : data)
        {
            resultFile << point.id << ",";
            for (auto attr : point.attributes)
            {
                resultFile << attr << ",";
            }
            resultFile << point.lengthOfVector << ",";
            resultFile << point.clasterId << ",";
            resultFile << point.pointType << ",";
            resultFile << point.timeToFindNeighbour << ",";
            resultFile << point.numbersOfDistanceCalculatons << ",";
            resultFile << point.rnnk << ",";
            resultFile << "[,";
            for (auto nn : point.nnk)
            {
                resultFile << nn << ";";
            }
            resultFile << "],";

            resultFile << "[,";
            for (auto rn : point.reverseNeighbourIndexes)
            {
                resultFile << rn << ";";
            }
            resultFile << "],";

            resultFile << "[,";
            for (auto index : point.neighbourIndexes)
            {
                resultFile << data[index].id << ";";
            }
            resultFile << "],";

            resultFile << "\n";
        }
        resultFile.close();
    }
    else
        cout << "Unable to open file";
}

void DataWriter::writeClasteringResults(string pathToFile, vector<MDPoint> &data)
{
    ofstream resultFile(pathToFile);

    if (resultFile.is_open())
    {
        resultFile << "Id,";
        int iter = 0;
        MDPoint temp = data[0];
        for (auto attr : temp.attributes)
        {
            resultFile << "x" << iter << ",";
            iter++;
        }
        resultFile << "clasterId,";
        resultFile << "pointType,";
        resultFile << "#dist,";
        resultFile << endl;

        for (auto point : data)
        {
            resultFile << point.id << ",";
            for (auto attr : point.attributes)
            {
                resultFile << attr << ",";
            }

            resultFile << point.clasterId << ",";
            resultFile << point.pointType << ",";
            resultFile << point.numbersOfDistanceCalculatons;
            resultFile << "\n";
        }
        resultFile.close();
    }
    else
        cout << "Unable to open file";
}

STAT::STAT(string pathToFile) : pathToFile(pathToFile){};
void STAT::writeLine(string line)
{
    ofstream resultFile(this->pathToFile, ios_base::app);

    if (resultFile.is_open())
    {
        resultFile << line << endl;
    }
    else
        cout << "Unable to open file";
}
void STAT::gatherDataFromCollection(vector<MDPoint> &data)
{
    this->numberOfPoints = data.size();
    this->numberofDismensions = data[0].attributes.size();
    int numberOfClusters=0;
    int numberOfCorePoints=0;
    int numberOfBorderPoints=0;
    int numberOfNoisePoint=0;
    int sumOfDistanceCalculations;
    long currentMax = 0;
    for(auto point: data){
        if(point.pointType==1){
            numberOfCorePoints++;
        }
        if(point.pointType==0){
            numberOfBorderPoints++;
        }
        if(point.pointType==-1){
            numberOfNoisePoint++;
        }
        if(point.clasterId>currentMax){
            numberOfClusters=point.clasterId;
            currentMax = point.clasterId;
        }
        sumOfDistanceCalculations+=point.numbersOfDistanceCalculatons;
    }
    this->numberOfClusters=numberOfClusters;
    this->numberOfCorePoints=numberOfCorePoints;
    this->numberOfBorderPoints=numberOfBorderPoints;
    this->numberOfNoisePoint=numberOfNoisePoint;
    this->avarOfDistanceCalculations=sumOfDistanceCalculations/this->numberOfPoints;

};
void STAT::writeSTATFile()
{
    ofstream resultFile(this->pathToFile);

    if (resultFile.is_open())
    {
        resultFile << "Algorithm = " << this->algorithm << endl;
        resultFile << "params = " << this->params<<endl;
        resultFile << "name of input file = " << this->nameOfInputFile << endl;
        resultFile << "number of points = " << this->numberOfPoints << endl;
        resultFile << "number of dimensions = " << this->numberofDismensions << endl;
        resultFile << "reading data set time [second]/[minuts] = " << this->readingDatasetTime / 1000 << "/" << this->readingDatasetTime / 60 / 1000 << endl;
        resultFile << "sorting data time [second]/[minuts] = " << this->sortingDataTime / 1000 << "/" << this->sortingDataTime / 60 / 1000 << endl;
        resultFile << "finding neighbours and reverse neighbours time [second]/[minuts] = " << this->findingNeighboursAndReverNeighbourTime / 1000 << "/" << this->findingNeighboursAndReverNeighbourTime / 60 / 1000 << endl;
        resultFile << "discover clasters time [second]/[minuts]= " << this->clasteringTime / 1000 << "/" << this->clasteringTime / 1000 / 60 << endl;
        resultFile << "total run time [second]/[minuts]= " << this->totalRuntime / 1000 << "/" << this->totalRuntime / 1000 / 60 << endl;
        resultFile << "number of clasters = " << this->numberOfClusters << endl;
        resultFile << "number of core points = " << this->numberOfCorePoints << endl;
        resultFile << "number of border points = " << this->numberOfBorderPoints << endl;
        resultFile << "number of noise points = " << this->numberOfNoisePoint << endl;
        resultFile << "avarage number of distance calculations = " << this->avarOfDistanceCalculations << endl;
    }
    else
        cout << "Unable to open file";
}