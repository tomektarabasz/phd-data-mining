#include "tanimpotoDist.h"
#include <stdexcept>
#include <math.h>
#include <numeric>
#include "vectorLength.h"
#include "../models/mdPoint.h"

double tanimotoDist(MDPoint Point1, MDPoint Point2)
{
    double similarity;
    size_t sizeOfPoint1 = Point1.attributes.size();
    size_t sizeOfPoint2 = Point2.attributes.size();
    if (sizeOfPoint1 != sizeOfPoint2)
    {
        return -1.0;
        throw std::invalid_argument("Vectors are not the same size");
    }
    double numerator = inner_product(Point1.attributes.begin(), Point1.attributes.end(), Point2.attributes.begin(), 0.0);
    double point1Length = pow(Point1.lengthOfVector, 2);
    double point2Length = pow(Point2.lengthOfVector, 2);
    double denominator = point1Length + point2Length - numerator;

    similarity = numerator / denominator;
    // WARNING
    // taniomoto is the similarity measures limited by 1. I want have disimilarity measure which will works as dist
    // thats why I btructing tanimoto similarity value from 1 which is max value for this similarity.
    return 1 - similarity;
};

double tanimotoAplhaCoefficient(double eps, bool isMin)
{
    double beta = 1.0 + 1.0 / eps;
    if (isMin)
    {
        return (beta - sqrt(pow(beta, 2.0) - 4.0)) / 2.0;
    }
    return (beta + sqrt(pow(beta, 2.0) - 4.0)) / 2.0;
}

TanimotoLengthRange vectorLengthRange(double vectorLength, double tanimotoSimilarity)
{
    double alphaMin = tanimotoAplhaCoefficient(tanimotoSimilarity, true);
    double alphaMax = tanimotoAplhaCoefficient(tanimotoSimilarity, false);
    TanimotoLengthRange result;
    result.minLength = vectorLength * alphaMin;
    result.maxLength = alphaMax * vectorLength;
    return result;
}