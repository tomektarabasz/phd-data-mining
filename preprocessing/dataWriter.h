#ifndef DATAWRITER_H
#define DATAWRITER_H
#include "../models/point.h"
#include "../models/mdPoint.h"
#include <vector>
#include <string>

using namespace std;

class DataWriter
{
public:
    static void write(string pathToFile, vector<Point>& data);
    static void writeMDPoints(string pathToFile, vector<MDPoint>& data);
    ~DataWriter();
};
#endif