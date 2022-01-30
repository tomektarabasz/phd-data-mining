#ifndef NAIVEREVERSENEARESNEIGHBOUR_H
#define NAIVEREVERSENEARESNEIGHBOUR_H
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include "../models/mdPoint.h"
#include "../preprocessing/dataWriter.h"

using namespace std;

void NaiveRNN(vector<MDPoint> &data, int k, double& executionTime);

#endif
