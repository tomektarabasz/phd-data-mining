#ifndef POINT_H
#define POINT_H
#include <string>
#include <vector>

using namespace std;

class Point {
    public:
    unsigned long guid;
    double x1;
    double x2;
    long clasterId;
    int neighbourNumber;
    Point(long guid,double x1, double x2);
    Point(string rowData);
    double distToPoint(Point& Point);
};

vector<int> findNeigbour(Point& point, vector<Point>& collection, double eps);
void assingClusterId(vector<int>,vector<Point>& collection, long clusterId);
#endif