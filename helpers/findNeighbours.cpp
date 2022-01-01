#include "findNeighbours.h"

void findNeigbour(MDPoint &point, vector<MDPoint> &collection, double eps)
{
    for (unsigned long i = 0; i < collection.size(); i++)
    {
        double dist = point.distToPoint(collection[i]);
        if (dist <= eps && point.id != collection[i].id)
        {
            point.neighbourIndexes.push_back(i);
        }
    }
}