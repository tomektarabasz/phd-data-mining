#include <iostream>
#include "mdPointLoader.h"
#include "../models/mdPoint.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

MDPointLoader::~MDPointLoader(){}

vector<MDPoint> MDPointLoader::getData(string pathToData)
{
    // Create a text string, which is used to output the text file
    string rowTextLine;

    // Read from the text file
    ifstream MyReadFile(pathToData);
    vector<MDPoint> dataEntityArray;

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, rowTextLine))
    {
        // Output the text from the file
        // cout << rowTextLine << endl;
        MDPoint dataEntity(rowTextLine);
        dataEntityArray.push_back(dataEntity);
    };
    // Close the file
    MyReadFile.close();
    return dataEntityArray;
}