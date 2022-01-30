#ifndef OPTIMISEDREVERSENEARESNEIGHBOUR_H
#define OPTIMISEDREVERSENEARESNEIGHBOUR_H
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include "../models/mdPoint.h"
#include "../preprocessing/dataWriter.h"

using namespace std;

void OptimisedRNN(vector<MDPoint> &data, int k, double& executionTime);

#endif
