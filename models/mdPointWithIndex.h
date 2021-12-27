#ifndef MDPOINTWITHINDEX_H
#define MDPOINTWITHINDEX_H
#include <string>
#include <vector>
#include "mdPoint.h"

using namespace std;

class MDPointWithIndex {
    public:
    MDPoint point;
    long index;
    double dist;
    MDPointWithIndex(MDPoint point, long index);
};
#endif