#include "tanimpotoDist.h"
#include <stdexcept>
#include <math.h>

double tanimotoDist(vector<double> Point1, vector<double> Point2)
{
    double dist;
    size_t sizeOfPoint1 = Point1.size();
    size_t sizeOfPoint2 = Point2.size();
    if (sizeOfPoint1 != sizeOfPoint2)
    {
        return -1.0;
        throw std::invalid_argument("Vectors are not the same size");
    }
    double numerator = 0;
    double denominator = 0;
    double x;
    double y;
    for (int i = 0; i < sizeOfPoint1; i++)
    {
        x = Point1[i];
        y = Point2[i];
        numerator += x * y;
        denominator += pow(x, 2) + pow(y, 2) - x * y;
    }
    dist = numerator / denominator;
    // WARNING 
    // taniomoto is the similarity measures limited by 1. I want have disimilarity measure which will works as dist
    // thats why I btructing tanimoto similarity value from 1 which is max value for this similarity.
    return 1-dist;
};