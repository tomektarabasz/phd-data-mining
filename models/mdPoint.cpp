#include <iostream>
#include "mdPoint.h"
#include "../preprocessing/splitedStringResult.h"
#include "../helpers/tanimpotoDist.h"
#include "../helpers/euclideanDist.h"
#include <vector>
#include <algorithm>
#include <string>
#include "../helpers/timeMeasure.h"
#include "../helpers/vectorLength.h"
#include "../helpers/vectorSubrange.h"

using namespace std;

void kPlusElements(vector<MDPointWithIndex> &neighbour, MDPoint referencePoint, int k)
{
    vector<MDPointWithIndex> result;
    double distLimit = neighbour[k].point.distToPoint(referencePoint);
    copy_if(neighbour.begin(), neighbour.end(), back_inserter(result), [distLimit, &referencePoint](MDPointWithIndex point)
            { return point.point.id != referencePoint.id && point.point.distToPoint(referencePoint) <= distLimit; });
    neighbour = result;
}

void sortVector(vector<MDPointWithIndex> &neighbour, MDPoint referencePoint)
{
    for (auto &point : neighbour)
    {
        point.dist = point.point.distToPoint(referencePoint);
    }
    sort(neighbour.begin(), neighbour.end(), [](MDPointWithIndex &pointA, MDPointWithIndex &pointB)
         { return pointA.dist < pointB.dist; });
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
    // double dist = euclideanDist(*this, point2);
    double dist = tanimotoDist(*this, point2);
    return dist;
}

void MDPoint::calcNNk(int k, vector<MDPoint> &data, unsigned long index)
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
        this->neighbourIndexes.push_back(point.index);
    }
    for (auto point : neighbour)
    {
        data[point.index].rnnk += 1;
        data[point.index].reverseNeighbourIndexes.push_back(index);
    }
}

vector<MDPointWithIndex> findNeighbours(double eps, MDPoint referencePoint, vector<MDPoint> &data, vector<MDPointWithIndex> &neighbours)
{
    MDPointWithIndex lastPoint = neighbours.back();
    double minTanimotoSimilarity = 1 - lastPoint.point.distToPoint(referencePoint);
    if (eps >= minTanimotoSimilarity)
    {
        vector<MDPointWithIndex> newNeighbours;
        TanimotoLengthRange rangeLength;
        rangeLength = vectorLengthRange(referencePoint.lengthOfVector, minTanimotoSimilarity);
        newNeighbours = vectorSubrange(rangeLength, data);
        for (auto point : newNeighbours)
        {
            double similarityCandidate = 1 - point.point.distToPoint(referencePoint);
            bool isGreaterSimilarity = similarityCandidate > minTanimotoSimilarity;
            bool isInNeighbour = false;
            bool isReferencePoint = referencePoint.id == point.point.id;
            for (auto n : neighbours)
            {
                if (n.index == point.index)
                {
                    isInNeighbour = true;
                    break;
                }
            }
            if (isGreaterSimilarity && !isInNeighbour && !isReferencePoint)
            {
                neighbours.pop_back();
                neighbours.push_back(point);
                sortVector(neighbours, referencePoint);
                findNeighbours(similarityCandidate, referencePoint, data, neighbours);
            }
        }
    }
    return neighbours;
}

void MDPoint::optimCalcNNk(int k, vector<MDPoint> &data, unsigned long index)
{
    vector<MDPointWithIndex> neighbour;
    int iterator = 0;
    MDPoint tempPoint;
    double initialTanimotoSimilarity = 1.0;
    TanimotoLengthRange rangeLength;
    do
    {
        initialTanimotoSimilarity -= 0.1;
        rangeLength = vectorLengthRange(this->lengthOfVector, initialTanimotoSimilarity);
        neighbour = vectorSubrange(rangeLength, data);
    } while (neighbour.size() < k + 1 && initialTanimotoSimilarity > 0);
    sortVector(neighbour, *this);

    bool isNewTheSameAsOld = false;

    kPlusElements(neighbour, *this, k);

    auto result = findNeighbours(initialTanimotoSimilarity, *this, data, neighbour);

    for (auto p : result)
    {
        this->nnk.push_back(p.point.id);
        this->neighbourIndexes.push_back(p.index);
        data[p.index].rnnk += 1;
        data[p.index].reverseNeighbourIndexes.push_back(index);
    }
    return;
}

void MDPoint::assingToTheSameClusterId(vector<MDPoint> &data)
{
    for (auto index : this->neighbourIndexes)
    {
        MDPoint &tempPoint = data[index];
        if (tempPoint.clasterId == -1)
        {
            tempPoint.clasterId = this->clasterId;
        }
    }
};