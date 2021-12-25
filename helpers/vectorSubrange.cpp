#include "vectorSubrange.h"

vector<MDPointWithIndex> vectorSubrange(TanimotoLengthRange range, vector<MDPoint> sortedData)
{
    vector<MDPointWithIndex> subrange;
    MDPoint tempPoint;
    for (int iterator = 0; iterator < sortedData.size(); iterator++)
    {
        tempPoint = sortedData[iterator];
        bool isToLong = tempPoint.lengthOfVector > range.maxLength;
        if (tempPoint.lengthOfVector >= range.minLength && !isToLong)
        {
            MDPointWithIndex tempPointWithIndex = MDPointWithIndex(tempPoint, iterator);
            subrange.push_back(tempPointWithIndex);
        }
        else if (isToLong)
        {
            break;
        }
    }
    return subrange;
};