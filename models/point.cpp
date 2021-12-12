#include <iostream>
#include "./point.h"
#include <string>
#include "../preprocessing/splitedStringResult.h"
#include <vector>
#include <math.h>

using namespace std;

Point::Point(long guid, double x1, double x2) : guid(guid), x1(x1), x2(x2), clasterId(-1), neighbourNumber(-1){};
Point::Point(string rowData): clasterId(-1), neighbourNumber(-1)
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
    this->guid = stol(splitedVector[0]);
    this->x1 = stod(splitedVector[1]);
    this->x2 = stod(splitedVector[2]);

    cout << "//start//" << endl;
    cout << " giud = " << this->guid << endl;
    cout << "x1 = " << this->x1 << endl;
    cout << "x2 = " << this->x2 << endl;
    cout << "//end//" << endl;
};

double Point::distToPoint(Point &point)
{
    return sqrt(pow(this->x1 - point.x1, 2) + pow(this->x2 - point.x2, 2));
};

vector<int> findNeigbour(Point &point, vector<Point> &collection, double eps)
{
    vector<int> neighbours;
    for (int i = 0; i < collection.size(); i++)
    {
        double dist = point.distToPoint(collection[i]);
        if (dist <= eps)
        {
            neighbours.push_back(i);
        }
    }
    return neighbours;
};

void assingClusterId(vector<int> subset, vector<Point> &data, long clusterId)
{
    for (auto guid : subset)
    {
        if (data[guid].clasterId == -1)
        {
            data[guid].clasterId = clusterId;
        }
    };
}
