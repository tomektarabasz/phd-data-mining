#ifndef DATAREADERFROMCSV_H
#define DATAREADERFROMCSV_H
#include "../models/purity.h"
#include <vector>
#include <string>

using namespace std;

class DataReaderFromCSV
{
public:
    static vector<DataEntity> getData(string pathToFile);
    ~DataReaderFromCSV();
};
#endif