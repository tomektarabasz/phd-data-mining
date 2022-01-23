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
        resultFile << "len(rnnk),";
        resultFile << "kNN,";
        resultFile << "reverskNN,";
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
        resultFile << "claserId,";
        resultFile << "pointType,";
        resultFile << "dist,";
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

            resultFile << "|,";
            for (auto dist : point.distancesToOtherPoints)
            {
                resultFile << dist.id << "=" << dist.dist << ",";
            }
            resultFile << "|,";
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