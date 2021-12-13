#ifndef MDPOINTLOADER_H
#define MDPOINTLOADER_H
#include "../models/mdPoint.h"
#include <vector>
#include <string>

using namespace std;

class MDPointLoader
{
public:
    static vector<MDPoint> getData(string pathToFile);
    ~MDPointLoader();
};
#endif