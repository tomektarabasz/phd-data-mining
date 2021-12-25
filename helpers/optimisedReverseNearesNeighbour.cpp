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

void OptimisedRNN(vector<MDPoint> &data)
{
    string pathToStoreTimeOfExecution = "Data/time.csv";
    TimeWriter timeWriter(pathToStoreTimeOfExecution, "calcNNk , pure version");

    sort(data.begin(), data.end(), [](MDPoint &pointA, MDPoint &pointB)
         { return pointA.lengthOfVector < pointB.lengthOfVector; });

    for (auto point : data)
    {
        point.optimCalcNNk(4,data);
    }

    timeWriter.stop();
    timeWriter.writeTime();
}