#include "identyficator.h"
#include <sstream>

string Identyficator(string modelName, string nubmerOfNodes)
{
    string result;
    std::ostringstream oss;
    oss << modelName << "," << nubmerOfNodes;
    result = oss.str();
    return result;
}