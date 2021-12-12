#include <string>

using namespace std;

struct SplitedStringResult
{
    std::string firstPart;
    std::string rest;
};

SplitedStringResult splitWithFirstDeliminer(string input, string delimiter);