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
    string pathToStoreTimeOfExecution = "Data/time.csv";
    TimeWriter timeWriter(pathToStoreTimeOfExecution, "calcNNk , optimVersion");
    timeWriter.start();

    TimeWriter timeWriterSorter(pathToStoreTimeOfExecution, "calcNNk , Sort Data based on length");
    timeWriterSorter.start();
    sort(data.begin(), data.end(), [](MDPoint &pointA, MDPoint &pointB)
         { return pointA.lengthOfVector < pointB.lengthOfVector; });
    timeWriterSorter.stop();
    timeWriterSorter.writeTime();
    executionTime = timeWriterSorter.getTime();
    timeWriter.start();
    size_t index = 0;
    for (auto &point : data)
    {
        point.optimCalcNNk(k, data, index, executionTime);
        index++;
    }

    timeWriter.stop();
    timeWriter.writeTime();
}