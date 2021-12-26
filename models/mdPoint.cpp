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
#include "../helpers/vectorSubrange.h"

using namespace std;

void sortVector(vector<MDPointWithIndex> &neighbour, MDPoint referencePoint)
{
    sort(neighbour.begin(), neighbour.end(), [&referencePoint](MDPointWithIndex &pointA, MDPointWithIndex &pointB)
         { return pointA.point.distToPoint(referencePoint) < pointB.point.distToPoint(referencePoint); });
};

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
    double dist = tanimotoDist(*this, point2);
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
    sortVector(neighbour, *this);
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
            double greatesDist = neighbour.back().point.distToPoint(*this);
            if (tempDist <= greatesDist)
            {
                if (tempDist != greatesDist)
                {
                    neighbour.pop_back();
                }

                neighbour.push_back(MDPointWithIndex(tempPoint, iterator));
                sortVector(neighbour, *this);
            }
        }
    }
    for (auto point : neighbour)
    {
        this->nnk.push_back(point.point.id);
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
    double initialTanimotoSimilarity = 0.95;
    TanimotoLengthRange rangeLength;
    do
    {
        rangeLength = vectorLengthRange(this->lengthOfVector, initialTanimotoSimilarity);
        neighbour = vectorSubrange(rangeLength, data);
        initialTanimotoSimilarity -= 0.1;
    } while (neighbour.size() < k+1 && initialTanimotoSimilarity > 0);
    sortVector(neighbour, *this);
    iterator = 0;

    double tempDist = neighbour[k].point.distToPoint(*this);
    for (iterator = 1; iterator < neighbour.size(); iterator++)
    {
        MDPointWithIndex tempPointWithIndex = neighbour[iterator];
        if (iterator >= k+1)
        {
            double dist = tempPointWithIndex.point.distToPoint(*this);
            if (dist > tempDist)
            {
                break;
            }
        }
        this->nnk.push_back(tempPointWithIndex.point.id);
        data[tempPointWithIndex.index].rnnk += 1;
    }
    return;

    // copy_if(data.begin(), data.end(), back_inserter(potentialNeighbour), [&rangeLength](MDPoint i)
    //         { return i.lengthOfVector >= rangeLength.minLength && i.lengthOfVector <= rangeLength.maxLength; });
}