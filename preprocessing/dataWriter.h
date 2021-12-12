#ifndef DATAWRITER_H
#define DATAWRITER_H
#include "../models/point.h"
#include <vector>
#include <string>

using namespace std;

class DataWriter
{
public:
    static void write(string pathToFile, vector<Point>& data);
    ~DataWriter();
};
#endif