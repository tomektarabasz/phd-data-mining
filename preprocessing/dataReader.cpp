#include <iostream>
#include "dataReader.h"
#include "../models/purity.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

DataReaderFromCSV::~DataReaderFromCSV(){}

vector<DataEntity> DataReaderFromCSV::getData(string pathToData)
{
    // Create a text string, which is used to output the text file
    string rowTextLine;

    // Read from the text file
    ifstream MyReadFile(pathToData);
    vector<DataEntity> dataEntityArray;

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, rowTextLine))
    {
        // Output the text from the file
        cout << rowTextLine << endl;
        DataEntity dataEntity(rowTextLine);
        dataEntityArray.push_back(dataEntity);
    };
    // Close the file
    MyReadFile.close();
    return dataEntityArray;
}