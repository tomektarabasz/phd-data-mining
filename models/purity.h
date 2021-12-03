#ifndef PURITY_H
#define PURITY_H
#include <string>
#include <vector>
#include <map>
using namespace std;
class Purity
{
public:
    Purity();
    Purity(int inputId, int outputId);
    int outputId;
    int inputId;
    void show();
};
class DataEntity
{
public:
    string guid;
    int realClusterId;
    int outputClusterId;
    DataEntity(std::string rowData);
    bool operator==(const DataEntity &other);
    bool operator!=(const DataEntity &other);
};

class OutputClaster
{
public:
    OutputClaster();
    OutputClaster(map<int, int> dictOfCountedAppeariance);
    vector<int> realeClastersArray;
    map<int, int> dictOfCountedAppeariance;
    int theMostFrequent;
    int calcTheMostFequent();
    int numberOfOccurenceForTheMostFrequent;
};

map<int, OutputClaster> makeMapFromData(vector<DataEntity> data);
#endif