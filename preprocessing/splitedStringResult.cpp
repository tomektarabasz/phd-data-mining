#include "splitedStringResult.h"


SplitedStringResult splitWithFirstDeliminer(string input, string delimiter)
{
    SplitedStringResult result;
    size_t pos = 0;
    if ((pos = input.find(delimiter)) != string::npos)
    {
        result.firstPart = input.substr(0, pos);
        result.rest = input.substr(pos+1, input.length());
    }
    else
    {
        result.firstPart = input;
        result.rest = "";
    }
    return result;
};