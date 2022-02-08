#include "findNeighbours.h"
#include "timeMeasure.h"

void findNeigbour(MDPoint &point, vector<MDPoint> &collection, double eps)
{
    TimeWriter timer;
    timer.start();
    for (unsigned long i = 0; i < collection.size(); i++)
    {
        double dist = point.distToPoint(collection[i]);
        if (dist <= eps /* && point.id != collection[i].id */) // I commented && point.id != collection[i].id because point itself is calculated as neighbour
        {
            point.neighbourIndexes.push_back(i);
        }
    }
    timer.stop();
    double time = timer.getTime();
    point.timeToFindNeighbour += time;
}