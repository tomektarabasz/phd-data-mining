#include "tanimpotoDist.h"
#include <stdexcept>
#include <math.h>
#include <numeric>

double tanimotoDist(vector<double> Point1, vector<double> Point2)
{
    double similarity;
    size_t sizeOfPoint1 = Point1.size();
    size_t sizeOfPoint2 = Point2.size();
    if (sizeOfPoint1 != sizeOfPoint2)
    {
        return -1.0;
        throw std::invalid_argument("Vectors are not the same size");
    }
    double numerator = inner_product(Point1.begin(), Point1.end(), Point2.begin(), 0);
    double point1Length = inner_product(Point1.begin(), Point1.end(), Point1.begin(), 0);
    double point2Length = inner_product(Point2.begin(), Point2.end(), Point2.begin(), 0);
    double denominator = point1Length + point2Length - numerator;
    double x;
    double y;

    similarity = numerator / denominator;
    // WARNING
    // taniomoto is the similarity measures limited by 1. I want have disimilarity measure which will works as dist
    // thats why I btructing tanimoto similarity value from 1 which is max value for this similarity.
    return 1 - similarity;
};

double tanimotoAplhaCoefficient(double eps)
{
    double beta = 1 + 1 / eps;
    return 1 / 2 * (beta + sqrt(pow(beta, 2) - 4));
}

TanimotoLengthRange vectorLengthRange(double vectorLength, double tanimotoSimilarity)
{
    double alpha = tanimotoAplhaCoefficient(tanimotoSimilarity);
    TanimotoLengthRange result;
    result.minLength =vectorLength / alpha;
    result.maxLength = alpha * vectorLength;
    return result;
}