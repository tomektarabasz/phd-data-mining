#ifndef MDPOINT_H
#define MDPOINT_H
#include <string>
#include <vector>

using namespace std;

class DistToPoint
{
public:
    string id;
    unsigned long guid;
    double dist;
    DistToPoint(string id, unsigned long guid, double dist);
};
class MDPoint
{
public:
    string id;
    unsigned long guid;
    vector<double> attributes;
    long clasterId;
    int pointType;
    double timeToFindNeighbour;
    vector<string> nnk;
    vector<unsigned long> neighbourIndexes;
    vector<unsigned long> reverseNeighbourIndexes;
    int numbersOfDistanceCalculatons;
    // vector<unsigned long> rnnk;
    long rnnk;
    double lengthOfVector;
    MDPoint(string rowData);
    MDPoint();
    double distToPoint(MDPoint &Point);
    void calcNNk(int k, vector<MDPoint> &data, unsigned long index, double &executiontTime);
    void optimCalcNNk(int k, vector<MDPoint> &data, unsigned long index, double &executiontTime);
    void assingToTheSameClusterId(vector<MDPoint> &data);
    void increaseNumberOfDistanceCalculations();
};
#endif