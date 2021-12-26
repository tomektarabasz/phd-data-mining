#ifndef MDPOINT_H
#define MDPOINT_H
#include <string>
#include <vector>

using namespace std;

class MDPoint
{
public:
    string id;
    unsigned long guid;
    vector<double> attributes;
    long clasterId;
    vector<string> nnk;
    // vector<unsigned long> rnnk;
    long rnnk;
    double lengthOfVector;
    MDPoint(string rowData);
    MDPoint();
    double distToPoint(MDPoint &Point);
    void calcNNk(int k, vector<MDPoint> &data);
    void optimCalcNNk(int k, vector<MDPoint> &data);
};
#endif