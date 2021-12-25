#ifndef TANIMOTO_H
#define TANIMOTO_H
#include <vector>

using namespace std;

struct TanimotoLengthRange
{
    double minLength;
    double maxLength;
};

double tanimotoDist(vector<double> Point1, vector<double> Point2);
double tanimotoAplhaCoefficient(double eps);
TanimotoLengthRange vectorLengthRange(double pointLength, double tanimotoSimilarity);

#endif
