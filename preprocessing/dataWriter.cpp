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
        for (auto point : data)
        {
            resultFile << point.id << ",";
            for (auto attr : point.attributes)
            {
                resultFile << attr << ",";
            }
            resultFile << "[,";
            for (auto nn : point.nnk)
            {
                resultFile << nn << ";";
            }
            resultFile << "],";
            resultFile << point.rnnk << ",";
            resultFile << "[,";
            for (auto rn : point.reverseNeighbourIndexes)
            {
                resultFile << rn << ";";
            }
            resultFile << "],";
            resultFile << point.lengthOfVector << ",";
            resultFile << point.clasterId << "\n";
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
        for (auto point : data)
        {
            resultFile << point.id << ",";
            for (auto attr : point.attributes)
            {
                resultFile << attr << ",";
            }
            resultFile << point.clasterId << "\n";
        }
        resultFile.close();
    }
    else
        cout << "Unable to open file";
}