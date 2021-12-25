#ifndef VECTORSUBRANGE_H
#define VECTORSUBRANGE_H
#include <vector>
#include "tanimpotoDist.h"
#include "../models/mdPointWithIndex.h"

using namespace std;

vector<MDPointWithIndex> vectorSubrange(TanimotoLengthRange range, vector<MDPoint> sortedData);

#endif
