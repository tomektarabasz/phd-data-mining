#ifndef POINTLOADER_H
#define POINTLOADER_H
#include "../models/point.h"
#include <vector>
#include <string>

using namespace std;

class PointLoader
{
public:
    static vector<Point> getData(string pathToFile);
    ~PointLoader();
};
#endif