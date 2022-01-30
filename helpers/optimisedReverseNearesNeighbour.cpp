#include "optimisedReverseNearesNeighbour.h"
#include "../models/mdPointWithIndex.h"
#include "timeMeasure.h"
#include "vectorLength.h"
#include <algorithm>

class MDPointWithDist : public MDPointWithIndex
{
public:
    double distToReferencePoint;
    MDPointWithDist(MDPoint point, long index, double dist) : MDPointWithIndex(point, index), distToReferencePoint(dist){};
};

void OptimisedRNN(vector<MDPoint> &data, int k, double &executionTime)
{
    TimeWriter timer;

    timer.start();
    sort(data.begin(), data.end(), [](MDPoint &pointA, MDPoint &pointB)
         { return pointA.lengthOfVector < pointB.lengthOfVector; });
    timer.stop();
    executionTime = timer.getTime();
    size_t index = 0;
    for (auto &point : data)
    {
        timer.start();
        point.optimCalcNNk(k, data, index, executionTime);
        timer.stop();
        point.timeToFindNeighbour = timer.getTime();
        index++;
    }
}