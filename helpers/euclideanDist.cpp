#include "euclideanDist.h"
#include "vectorLength.h"

double euclideanDist(MDPoint point1, MDPoint point2)
{
    MDPoint delta;
    for (int i = 0; i < point2.attributes.size(); i++)
    {
        delta.attributes.push_back(point1.attributes[i] - point2.attributes[i]);
    }
    double dist = vectorLength(delta.attributes);
    return dist;
};
