#include "vectorLength.h"
#include <numeric>
#include <math.h>

double vectorLength(vector<double> attributes)
{
    double sum = 0;
    for (auto attr : attributes)
    {
        sum += pow(attr, 2);
    }
    double tt = sqrt(sum);
    return sqrt(sum);
}