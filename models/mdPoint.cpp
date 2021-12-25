#include <iostream>
#include "mdPoint.h"
#include "../preprocessing/splitedStringResult.h"
#include "../helpers/tanimpotoDist.h"
#include <vector>
#include <algorithm>
#include <string>
#include "../helpers/timeMeasure.h"
#include "mdPointWithIndex.h"
#include "../helpers/vectorLength.h"

using namespace std;

MDPoint::MDPoint(string rowData) : clasterId(-1), rnnk(0)
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
    this->lengthOfVector = vectorLength(this->attributes);
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
    vector<MDPointWithIndex> neighbour;
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
            neighbour.push_back(MDPointWithIndex(tempPoint, iterator));
            iterator++;
        }
    }

    sort(neighbour.begin(), neighbour.end(), [this](MDPointWithIndex &pointA, MDPointWithIndex &pointB)
         { return pointA.point.distToPoint(*this) < pointB.point.distToPoint(*this); });

    for (; iterator < data.size(); iterator++)
    {
        tempPoint = data[iterator];
        if (this->guid == tempPoint.guid)
        {
            continue;
        }
        else
        {
            double tempDist = tempPoint.distToPoint(*this);
            if (tempDist < neighbour.back().point.distToPoint(*this))
            {
                neighbour.pop_back();
                neighbour.push_back(MDPointWithIndex(tempPoint, iterator));
                sort(neighbour.begin(), neighbour.end(), [this](MDPointWithIndex &pointA, MDPointWithIndex &pointB)
                     { return pointA.point.distToPoint(*this) < pointB.point.distToPoint(*this); });
            }
        }
    }
    for (auto point : neighbour)
    {
        this->nnk.push_back(point.point.guid);
    }
    for (auto point : neighbour)
    {
        data[point.index].rnnk += 1;
    }
}

void MDPoint::optimCalcNNk(int k, vector<MDPoint> &data)
{
    vector<MDPointWithIndex> neighbour;
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
            neighbour.push_back(MDPointWithIndex(tempPoint, iterator));
            iterator++;
        }
    }

    sort(neighbour.begin(), neighbour.end(), [this](MDPointWithIndex &pointA, MDPointWithIndex &pointB)
         { return pointA.point.distToPoint(*this) < pointB.point.distToPoint(*this); });

    tempPoint = neighbour.back().point;
    TanimotoLengthRange rangeLength;

    rangeLength =  vectorLengthRange(this->lengthOfVector,1-tempPoint.distToPoint(*this));

    for (auto point : neighbour)
    {
        this->nnk.push_back(point.point.guid);
    }
    for (auto point : neighbour)
    {
        data[point.index].rnnk += 1;
    }
}