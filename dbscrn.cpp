#include <string>
#include "models/mdPoint.h"
#include <vector>
#include <algorithm>
#include "preprocessing/mdPointLoader.h"
#include "helpers/timeMeasure.h"
#include "helpers/identyficator.h"
#include "helpers/naiveReverseNearesNeighbour.h"
#include "helpers/optimisedReverseNearesNeighbour.h"
#include "./preprocessing/dataWriter.h"

using namespace std;

void filterNeighbourFromAlreadyCalculatedPoints(vector<unsigned long> &seed, vector<MDPoint> &allData)
{
    vector<unsigned long> result;
    copy_if(seed.begin(), seed.end(), back_inserter(result), [&allData](unsigned long index)
            { return allData[index].clasterId == -1; });
    seed = result;
}

void buildClaster(vector<unsigned long> potentialSeeds, vector<MDPoint> &allData, int k, long clasterId)
{
    cout<<"buildClaster"<<endl;
    if (potentialSeeds.size() < 1)
    {
        return;
    }
    for (auto index : potentialSeeds)
    {
        MDPoint& p = allData[index];
        if (p.clasterId == -1)
        {
            p.clasterId = clasterId;
        }
    }
    MDPoint &point = allData[potentialSeeds.back()];

    if (point.rnnk >= k)
    {
        vector<unsigned long> newPotentialSeeds = point.reverseNeighbourIndexes;
        filterNeighbourFromAlreadyCalculatedPoints(newPotentialSeeds, allData);
        // point.assingToTheSameClusterId(allData);
        buildClaster(newPotentialSeeds, allData, k, clasterId);
    };
    potentialSeeds.pop_back();
    buildClaster(potentialSeeds, allData, k, clasterId);
}

void enterToBuildClaster(vector<MDPoint>& data, int k){
    unsigned long currentClasterId = 1;

    for (auto &currentPoint : data)
    {
        if (currentPoint.clasterId = -1)
        {
            if (currentPoint.rnnk > k)
            {
                currentPoint.clasterId = currentClasterId;
                buildClaster(currentPoint.neighbourIndexes, data, k, currentClasterId);
                currentClasterId++;
            }
        }
    }
}

int main()
{
    string pathToFile = "Data/lecture.csv";
    // string pathToFile = "Data/points.csv";
    // string pathToFile = "Data/dim512.csv";
    // string pathToFile = "Data/complex9.csv";
    // string pathToFile = "Data/cluto-t7-10k.csv";
    // string pathToFile = "Data/letter.csv";

    string pathToStoreTimeOfExecution = "Data/time.csv";

    //Data access
    TimeWriter *tempTimeWirter = new TimeWriter(pathToStoreTimeOfExecution, "dataTimeLoading");
    tempTimeWirter->start();
    MDPointLoader dataReader;
    vector<MDPoint> *data = new vector<MDPoint>(dataReader.getData(pathToFile));
    vector<MDPoint> &dataR = *data;
    tempTimeWirter->stop();
    tempTimeWirter->writeTime();
    delete tempTimeWirter;
    //End Data access

    //Start procedure
    TimeWriter timeWriter(pathToStoreTimeOfExecution, Identyficator("dbscrn", to_string(dataR.size())));
    int k = 4;
    timeWriter.start();

    vector<MDPoint> dataOptim = *data;
    vector<MDPoint> &dataOptimR = dataOptim;
    NaiveRNN(dataR, k);
    DataWriter dataWriter;
    string pathToResultFile = "Data/dbscrnResults.csv";
    string pathToDebugFile = "Data/naiveVersionDebug.csv";
    dataWriter.writeMDPoints(pathToDebugFile, dataR);

    OptimisedRNN(dataOptimR, k);
    pathToResultFile = "Data/optimVersionDebug.csv";

    //Clasters building
    enterToBuildClaster(dataOptimR,k);
    // This it end and time calculation
    timeWriter.stop();
    timeWriter.writeTime();
    dataWriter.writeMDPoints(pathToDebugFile,dataOptimR);
    //End Claster building
    dataWriter.writeClasteringResults(pathToResultFile, dataOptimR);
    dataWriter.writeClasteringResults(pathToResultFile,dataR);
    

    return 0;
}