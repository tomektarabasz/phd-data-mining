#include <iostream>
#include "models/purity.h"
#include "preprocessing/dataReader.h"
#include <vector>
#include <map>

using namespace std;

int sumUpValuesInVector(vector<int> vector){
    int result = 0;
    for(int x : vector){
        result+=x;
    };
    return result;
}

int main()
{
    cout << "Hellow Word" << endl;
    //Read data
    string pathToFile = "Data/purity.csv";
    DataReaderFromCSV dataReader;
    vector<DataEntity>* data = new vector<DataEntity>(dataReader.getData(pathToFile));
    // Main process
    map<int, OutputClaster> mapOfOutputClasters;
    mapOfOutputClasters = makeMapFromData(*data);
    delete data;
    vector<int> partialResult;
    for(auto item : mapOfOutputClasters){
        int hash = item.second.calcTheMostFequent();
        int numberOfOccurence = item.second.dictOfCountedAppeariance[hash];
        partialResult.push_back(numberOfOccurence);
    };
    int result = sumUpValuesInVector(partialResult);
    //Write data
    cout<< "This is result = " << result<< endl;
    return 0;
};
