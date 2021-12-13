#include <iostream>
#include "mdPoint.h"
#include "../preprocessing/splitedStringResult.h"
#include "../helpers/tanimpotoDist.h"
#include <vector>
#include <algorithm>
#include <string>
#include "../helpers/timeMeasure.h"

using namespace std;

MDPoint::MDPoint(string rowData) : clasterId(-1)
{
    size_t pos = 0;
    string delimiter = ",";
    SplitedStringResult splitedString;
    vector<string> splitedVector;
    do
    {
        splitedString = splitWithFirstDeliminer(rowData, delimiter);
        splitedVector.push_back(splitedString.firstPart);
        rowData = splitedString.rest;
    } while (splitedString.rest != "");
    this->id = splitedVector[0];
    this->guid = hash<string>()(splitedVector[0]);
    for (int i = 1; i < splitedVector.size(); i++)
    {
        double attr = stod(splitedVector[i]);
        this->attributes.push_back(attr);
    }
    cout << "//start//" << endl;
    cout << "id = " << this->id << endl;
    cout << " giud = " << this->guid << endl;
    cout << "//end//" << endl;
};

MDPoint::MDPoint(){};

double MDPoint::distToPoint(MDPoint &point2)
{
    double dist = tanimotoDist(this->attributes, point2.attributes);
    return dist;
}

void MDPoint::calcNNk(int k, vector<MDPoint> &data)
{
    string pathToStoreTimeOfExecution = "Data/time.csv";
    TimeWriter timeWriter(pathToStoreTimeOfExecution,"calcNNk , pure version");
    timeWriter.start();

    vector<MDPoint> neighbour;
    int iterator = 0;
    MDPoint tempPoint;
    while (neighbour.size() < k)
    {
        tempPoint = data[iterator];
        if (this->guid == tempPoint.guid)
        {
            iterator++;
            continue;
        }
        else
        {
            neighbour.push_back(tempPoint);
            iterator++;
        }
    }

    sort(neighbour.begin(), neighbour.end(), [this](MDPoint &pointA, MDPoint &pointB)
         { return pointA.distToPoint(*this) < pointB.distToPoint(*this); });

    for (size_t i = iterator; i < data.size(); i++)
    {
        tempPoint = data[iterator];
        if (this->guid == tempPoint.guid)
        {
            continue;
        }
        else
        {
            double tempDist = tempPoint.distToPoint(*this);
            if (tempDist < neighbour.back().distToPoint(*this))
            {
                neighbour.pop_back();
                neighbour.push_back(tempPoint);
                sort(neighbour.begin(), neighbour.end(), [this](MDPoint &pointA, MDPoint &pointB)
                     { return pointA.distToPoint(*this) < pointB.distToPoint(*this); });
            }
        }
    }
    for(auto point : neighbour){
        this->nnk.push_back(point.guid);
    }
    timeWriter.stop();
    timeWriter.writeTime();
}