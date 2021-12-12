#include <iostream>
#include "models/purity.h"
#include "preprocessing/pointLoader.h"
#include "./preprocessing/dataWriter.h"
#include <vector>
#include <map>
#include "models/point.h"

using namespace std;

void filterNeighbour(vector<int> &neighbour, vector<Point> &allData)
{
    Point point(0, 0, 0);
    vector<int> result;
    for (auto i : neighbour)
    {
        point = allData[i];
        if (point.neighbourNumber == -1)
        {
            result.push_back(i);
        };
    };
    neighbour = result;
};

void buildClaster(vector<int> &seeds, vector<Point> &allData, int numberOfMinNeighbour, long clasterId, double eps)
{
    if (seeds.size() < 1)
    {
        return;
    }
    vector<int> neighbours;
    Point& point = allData[seeds.back()];

    neighbours = findNeigbour(point, allData, eps);
    point.neighbourNumber = neighbours.size();
    if (neighbours.size() >= numberOfMinNeighbour)
    {
        filterNeighbour(neighbours, allData);
        assingClusterId(neighbours, allData, clasterId);
        buildClaster(neighbours, allData, numberOfMinNeighbour, clasterId, eps);
    };
    seeds.pop_back();
    buildClaster(seeds, allData, numberOfMinNeighbour, clasterId, eps);
}

int main()
{
    cout << "Hellow Word" << endl;
    //Read data
    string pathToFile = "Data/points.csv";
    PointLoader dataReader;
    vector<Point> *data = new vector<Point>(dataReader.getData(pathToFile));
    vector<Point> &dataR = *data;
    // Main process
    int numberOfMinNeighbour = 4;
    double eps = 0.10;
    unsigned long currentClasterId = 1;
    vector<int> currentNeighbours;

    for (auto currentPoint : dataR)
    {
        if (currentPoint.clasterId == -1)
        {
            currentNeighbours = findNeigbour(currentPoint, dataR, eps);
            // if it it core assing cluster id
            if (currentNeighbours.size() >= numberOfMinNeighbour)
            {
                assingClusterId(currentNeighbours, dataR, currentClasterId);
                buildClaster(currentNeighbours, dataR, numberOfMinNeighbour, currentClasterId, eps);
                currentClasterId++;
            }
        }
    }
    DataWriter dataWriter;
    string pathToResultFile = "Data/dbscanResult.csv";
    dataWriter.write(pathToResultFile,dataR);
    delete data;

    return 0;
};
