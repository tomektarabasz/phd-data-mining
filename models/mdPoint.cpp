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

DistToPoint::DistToPoint(string id, unsigned long guid, double dist) : id(id), guid(guid), dist(dist){};

class IndexAndLength
{
public:
    long minIndex;
    long maxIndex;
    double minLength;
    double maxLength;
    IndexAndLength() {}
    IndexAndLength(vector<MDPointWithIndex> data)
    {
        this->maxIndex = data[0].index;
        this->minIndex = data[0].index;
        for (auto item : data)
        {
            if (this->maxIndex <= item.index)
            {
                this->maxIndex = item.index;
                this->maxLength = item.point.lengthOfVector;
            }
            if (this->minIndex >= item.index)
            {
                this->minIndex = item.index;
                this->minLength = item.point.lengthOfVector;
            }
        }
    }
};

void kPlusElements(vector<MDPointWithIndex> &neighbour, MDPoint referencePoint, int k)
{
    vector<MDPointWithIndex> result;
    double distLimit = referencePoint.distToPoint(neighbour[k].point);
    copy_if(neighbour.begin(), neighbour.end(), back_inserter(result), [distLimit, &referencePoint](MDPointWithIndex point)
            { return point.point.id != referencePoint.id && referencePoint.distToPoint(point.point) <= distLimit; });
    neighbour = result;
}

void sortVector(vector<MDPointWithIndex> &neighbour, MDPoint &referencePoint)
{
    for (auto &point : neighbour)
    {
        point.dist = referencePoint.distToPoint(point.point);
    }
    sort(neighbour.begin(), neighbour.end(), [](MDPointWithIndex &pointA, MDPointWithIndex &pointB)
         { return pointA.dist < pointB.dist; });
};

MDPoint::MDPoint(string rowData) : clasterId(-1), rnnk(0), pointType(-1)
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
    //Create element to add to distanceToOtherPoints
    DistToPoint tempDistToPoint(point2.id, point2.guid, dist);
    this->distancesToOtherPoints.push_back(tempDistToPoint);
    // end sectionF
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
            double tempDist = tempPoint.distToPoint(*this);
            //Create element to add to distanceToOtherPoints
            DistToPoint tempDistToPoint(tempPoint.id, tempPoint.guid, tempDist);
            this->distancesToOtherPoints.push_back(tempDistToPoint);
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
            //Create element to add to distanceToOtherPoints
            DistToPoint tempDistToPoint(tempPoint.id, tempPoint.guid, tempDist);
            this->distancesToOtherPoints.push_back(tempDistToPoint);
            // end section
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

// void MDPoint::optimCalcNNk(int k, vector<MDPoint> &data, unsigned long index)
// {
//     string pathToStoreTimeOfExecution = "Data/time.csv";
//     vector<MDPointWithIndex> neighbour;
//     int iterator = 0;
//     MDPoint tempPoint;
//     double initialTanimotoSimilarity = 1.0;
//     TanimotoLengthRange rangeLength;
//     TimeWriter timeWriterInitial(pathToStoreTimeOfExecution, "calcNNk , Finding first k - potentil neighbours");
//     timeWriterInitial.start();
//     do
//     {
//         initialTanimotoSimilarity -= 0.1;
//         rangeLength = vectorLengthRange(this->lengthOfVector, initialTanimotoSimilarity);
//         neighbour = vectorSubrange(rangeLength, data);
//     } while (neighbour.size() < k + 1 && initialTanimotoSimilarity > 0);
//     timeWriterInitial.stop();
//     timeWriterInitial.writeTime();

//     TimeWriter timeWriterSort(pathToStoreTimeOfExecution, "calcNNk , Sort neighbours");
//     timeWriterSort.start();

//     sortVector(neighbour, *this);

//     timeWriterSort.stop();
//     timeWriterSort.writeTime();

//     bool isNewTheSameAsOld = false;

//     TimeWriter timeWriterKPlus(pathToStoreTimeOfExecution, "calcNNk , k PlustElements function call which calculates tanimoto and sort neighbours");
//     timeWriterKPlus.start();

//     kPlusElements(neighbour, *this, k);

//     timeWriterKPlus.stop();
//     timeWriterKPlus.writeTime();

//     TimeWriter timeWriterFindsNeighbors(pathToStoreTimeOfExecution, "calcNNk , k PlustElements function call which calculates tanimoto and sort neighbours");
//     timeWriterFindsNeighbors.start();

//     auto result = findNeighbours(initialTanimotoSimilarity, *this, data, neighbour);

//     timeWriterFindsNeighbors.stop();
//     timeWriterFindsNeighbors.writeTime();

//     for (auto p : result)
//     {
//         this->nnk.push_back(p.point.id);
//         this->neighbourIndexes.push_back(p.index);
//         data[p.index].rnnk += 1;
//         data[p.index].reverseNeighbourIndexes.push_back(index);
//     }
//     return;
// }
template <typename T>
vector<T> sliceVector(vector<T> vec, unsigned long k) { return vector<T>(vec.begin(), vec.begin() + k); };

vector<MDPointWithIndex> kSouranders(int k, vector<MDPoint> &data, unsigned long index, IndexAndLength &alreadyChecked)
{
    MDPoint referencePoint = data[index];
    long minIndex = index - k;
    if (minIndex < 0)
    {
        minIndex = 0;
    }
    long maxIndex = index + k;
    long maxDataIndex = data.size() - 1;
    if (maxIndex > maxDataIndex)
    {
        maxIndex = maxDataIndex;
    }

    vector<MDPointWithIndex> candidates;

    for (auto i = minIndex; i <= maxIndex; i++)
    {
        if (i == index)
        {
            continue;
        }
        MDPointWithIndex temp = MDPointWithIndex(data[i], i);
        candidates.push_back(temp);
    }

    sort(candidates.begin(), candidates.end(), [&referencePoint](MDPointWithIndex &a, MDPointWithIndex &b)
         {
             double deltaA = abs(referencePoint.lengthOfVector - a.point.lengthOfVector);
             double deltaB = abs(referencePoint.lengthOfVector - b.point.lengthOfVector);
             return deltaA <= deltaB;
         });
    candidates = sliceVector<MDPointWithIndex>(candidates, k);

    alreadyChecked = IndexAndLength(candidates);
    return candidates;
}

bool isSubRange(TanimotoLengthRange &newRangeLength, IndexAndLength &alreadyChecked)
{
    return alreadyChecked.minLength <= newRangeLength.minLength && alreadyChecked.maxLength >= newRangeLength.maxLength;
}

bool isAllDataChecked(IndexAndLength &alreadyChecked, vector<MDPoint> &data)
{
    long minIndex = 0;
    long maxIndex = data.size() - 1;
    return alreadyChecked.maxIndex >= maxIndex && alreadyChecked.minIndex <= minIndex;
}

bool isValidNeighbour(double refDist, double newDist, long minOrMaxIndex, long referenceIndex, long newNeighbourIndex)
{
    return newDist < refDist && newNeighbourIndex != minOrMaxIndex && newNeighbourIndex != referenceIndex;
};

void MDPoint::optimCalcNNk(int k, vector<MDPoint> &data, unsigned long index)
{
    long maxIndex = data.size() - 1;
    string pathToStoreTimeOfExecution = "Data/time.csv";
    vector<MDPointWithIndex> neighbour;
    int iterator = 0;
    // initializing first sets of neighbours
    IndexAndLength alreadyChecked;
    neighbour = kSouranders(k, data, index, alreadyChecked);
    sortVector(neighbour, *this);
    MDPointWithIndex lastNeighbour = neighbour.back();
    double tanimotoSimilarity = 1 - lastNeighbour.dist;
    TanimotoLengthRange rangeLength = vectorLengthRange(this->lengthOfVector, tanimotoSimilarity);

    while (!isSubRange(rangeLength, alreadyChecked) && !isAllDataChecked(alreadyChecked, data))
    {
        long graterIndex = alreadyChecked.maxIndex + 1;
        if (graterIndex > maxIndex)
        {
            graterIndex = maxIndex;
        }
        long lessIndex = alreadyChecked.minIndex - 1;
        if (lessIndex < 0)
        {
            lessIndex = 0;
        }
        MDPointWithIndex nextPoint(data[graterIndex], graterIndex);
        MDPointWithIndex lessPoint(data[lessIndex], lessIndex);
        nextPoint.dist = this->distToPoint(nextPoint.point);
        lessPoint.dist = this->distToPoint(lessPoint.point);

        if (isValidNeighbour(lastNeighbour.dist, nextPoint.dist, alreadyChecked.maxIndex, index, nextPoint.index))
        {
            neighbour.push_back(nextPoint);
        }
        if (isValidNeighbour(lastNeighbour.dist, lessPoint.dist, alreadyChecked.minIndex, index, lessPoint.index))
        {
            neighbour.push_back(lessPoint);
        }
        sortVector(neighbour, *this);
        neighbour = sliceVector<MDPointWithIndex>(neighbour, k);
        alreadyChecked.maxIndex = graterIndex;
        alreadyChecked.maxLength = nextPoint.point.lengthOfVector;
        alreadyChecked.minIndex = lessIndex;
        alreadyChecked.minLength = lessPoint.point.lengthOfVector;
        lastNeighbour = neighbour.back();
        tanimotoSimilarity = 1 - lastNeighbour.dist;
        rangeLength = vectorLengthRange(this->lengthOfVector, tanimotoSimilarity);
        iterator++;
    }
    // end initialization

    for (auto p : neighbour)
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