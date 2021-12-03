#include <iostream>
#include "models/purity.h"
#include "preprocessing/dataReader.h"
#include <vector>
#include <map>

using namespace std;

int sumUpValuesInVector(vector<int> vector)
{
    int result = 0;
    for (int x : vector)
    {
        result += x;
    };
    return result;
}

long calculateVariance(long numberOfRecords, int numberInPairs = 2)
{

    if (numberOfRecords == numberInPairs || numberInPairs == 0)
    {
        return 1;
    }
    return calculateVariance(numberOfRecords - 1, numberInPairs - 1) * numberOfRecords / numberInPairs;
};

int main()
{
    cout << "Hellow Word" << endl;
    //Read data
    string pathToFile = "Data/purity.csv";
    DataReaderFromCSV dataReader;
    vector<DataEntity> *data = new vector<DataEntity>(dataReader.getData(pathToFile));
    // Main process
    map<int, OutputClaster> mapOfOutputClasters;
    mapOfOutputClasters = makeMapFromData(*data);
    long numberOfPossiblePairs = calculateVariance(data->size());
    delete data;
    // Calculating True Possitive
    vector<int> partialResult;
    for (auto item : mapOfOutputClasters)
    {
        int hash = item.second.calcTheMostFequent();
        int numberOfOccurence = item.second.dictOfCountedAppeariance[hash];
        partialResult.push_back(numberOfOccurence);
    };
    int truePositives = sumUpValuesInVector(partialResult);
    //Write data
    cout << "This is truePositives = " << truePositives << endl;
    // Calculating True Negative
    int trueNegative = 0;
    partialResult.clear();
    for (auto claster : mapOfOutputClasters)
    {
        for (auto claster2 : mapOfOutputClasters)
        {
            if (claster2.first == claster.first)
            {
                continue;
            }
            else
            {
                for (auto element : claster.second.realeClastersArray)
                {
                    for (auto element2 : claster2.second.realeClastersArray)
                    {
                        if (element == element2)
                        {
                            trueNegative++;
                        }
                    }
                };
            };
        }
    }

    cout << "This is trueNegative = " << trueNegative << endl;

    cout << "This is Rand value = " << trueNegative + truePositives << endl;
    return 0;
};
