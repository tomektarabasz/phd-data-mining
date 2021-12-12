#include <iostream>
#include "pointLoader.h"
#include "../models/point.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

PointLoader::~PointLoader(){}

vector<Point> PointLoader::getData(string pathToData)
{
    // Create a text string, which is used to output the text file
    string rowTextLine;

    // Read from the text file
    ifstream MyReadFile(pathToData);
    vector<Point> dataEntityArray;

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, rowTextLine))
    {
        // Output the text from the file
        cout << rowTextLine << endl;
        Point dataEntity(rowTextLine);
        dataEntityArray.push_back(dataEntity);
    };
    // Close the file
    MyReadFile.close();
    return dataEntityArray;
}