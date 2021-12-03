#include <iostream>
#include "purity.h"
#include <string>
#include <vector>

using namespace std;

Purity::Purity()
{
    cout << "output=" << (*this).outputId << "input=" << this->inputId;
};
Purity::Purity(int intputId, int outputId)
{
    this->outputId = outputId;
    this->inputId = inputId;
}
void Purity::show()
{
    cout << "output=" << outputId << "input=" << inputId;
}

struct SplitedStringResult
{
    std::string firstPart;
    std::string rest;
};

SplitedStringResult splitWithFirstDeliminer(string input, string delimiter)
{
    SplitedStringResult result;
    size_t pos = 0;
    if ((pos = input.find(delimiter)) != string::npos)
    {
        result.firstPart = input.substr(0, pos);
        result.rest = input.substr(pos+1, input.length());
    }
    else
    {
        result.firstPart = input;
        result.rest = "";
    }
    return result;
};

DataEntity::DataEntity(string rowData)
{

    size_t pos = 0;
    string delimiter = ",";
    SplitedStringResult splitedString;
    vector<string> splitedVector;
    do
    {
        splitedString = splitWithFirstDeliminer(rowData, delimiter);
        splitedVector.push_back(splitedString.firstPart);
        rowData=splitedString.rest;
    } while (splitedString.rest != "");

    this->guid = splitedVector[0];
    this->realClusterId = hash<string>()(splitedVector[1]);
    this->outputClusterId = hash<string>()(splitedVector[2]);

    cout << "//start//" << endl;
    cout << "DataEntity realeClusterId = " << DataEntity::guid << endl;
    cout << "DataEntity realeClusterId = " << DataEntity::realClusterId << endl;
    cout << "DataEntity outputClusterId = " << DataEntity::realClusterId << endl;
    cout << "//end//" << endl;
}
bool DataEntity::operator==(const DataEntity &other)
{
    return other.outputClusterId == this->outputClusterId;
}
bool DataEntity::operator!=(const DataEntity &other)
{
    return other.outputClusterId != this->outputClusterId;
}

OutputClaster::OutputClaster() : realeClastersArray(vector<int>()) {}
OutputClaster::OutputClaster(map<int, int> dictOfCountedAppeariance) : dictOfCountedAppeariance(dictOfCountedAppeariance){};
int OutputClaster::calcTheMostFequent()
{
    map<int, int> frequency;
    for (auto entity : this->realeClastersArray)
    {
        if (frequency.count(entity) > 0)
        {
            frequency[entity] += 1;
        }
        else
        {
            frequency[entity] = 1;
        };
    };
    this->dictOfCountedAppeariance = frequency;
    int result = -1;
    int currentMax = -1;
    for (auto x : frequency)
    {
        if (currentMax < x.second)
        {
            result = x.first;
            currentMax = x.second;
        };
    };
    this->theMostFrequent = result;
    numberOfOccurenceForTheMostFrequent = currentMax;
    return result;
}

map<int, OutputClaster> makeMapFromData(vector<DataEntity> data)
{
    map<int, OutputClaster> result;
    for (auto entity : data)
    {
        if (result.count(entity.outputClusterId) > 0)
        {
            result[entity.outputClusterId].realeClastersArray.push_back(entity.realClusterId);
        }
        else
        {
            auto newOutputClaster = new OutputClaster();
            result[entity.outputClusterId] = *newOutputClaster;
        };
    };
    return result;
};