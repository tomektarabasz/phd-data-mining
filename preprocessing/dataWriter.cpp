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
