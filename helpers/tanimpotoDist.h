#ifndef TANIMOTO_H
#define TANIMOTO_H
#include <vector>
#include "../models/mdPoint.h"

using namespace std;

struct TanimotoLengthRange
{
    double minLength;
    double maxLength;
};

double tanimotoDist(MDPoint Point1, MDPoint Point2);
double tanimotoAplhaCoefficient(double eps);
TanimotoLengthRange vectorLengthRange(double pointLength, double tanimotoSimilarity);

#endif
