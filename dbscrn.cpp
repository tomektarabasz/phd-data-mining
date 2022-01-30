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
#include <sstream>

using namespace std;

bool isEnughtRNNToBeCorePoint(long rnn, long limit)
{
    return rnn > limit;
};

void filterNeighbourFromAlreadyCalculatedPoints(vector<unsigned long> &seed, vector<MDPoint> &allData)
{
    vector<unsigned long> result;
    copy_if(seed.begin(), seed.end(), back_inserter(result), [&allData](unsigned long index)
            { return allData[index].clasterId == -1; });
    seed = result;
}

void buildClaster(vector<unsigned long> potentialSeeds, vector<MDPoint> &allData, int k, long clasterId)
{
    if (potentialSeeds.size() < 1)
    {
        return;
    }
    for (auto index : potentialSeeds)
    {
        MDPoint &p = allData[index];
        if (p.clasterId == -1)
        {
            p.pointType = 0;
            p.clasterId = clasterId;

            if (isEnughtRNNToBeCorePoint(p.rnnk, k))
            {
                p.pointType = 1;
                vector<unsigned long> newPotentialSeeds = p.reverseNeighbourIndexes;
                filterNeighbourFromAlreadyCalculatedPoints(newPotentialSeeds, allData);
                // point.assingToTheSameClusterId(allData);
                buildClaster(newPotentialSeeds, allData, k, clasterId);
            };
        }
    }
    // MDPoint &point = allData[potentialSeeds.back()];

    // if (point.rnnk >= k)
    // {
    //     vector<unsigned long> newPotentialSeeds = point.reverseNeighbourIndexes;
    //     filterNeighbourFromAlreadyCalculatedPoints(newPotentialSeeds, allData);
    //     // point.assingToTheSameClusterId(allData);
    //     buildClaster(newPotentialSeeds, allData, k, clasterId);
    // };
    // potentialSeeds.pop_back();
    // buildClaster(potentialSeeds, allData, k, clasterId);
}

void enterToBuildClaster(vector<MDPoint> &data, int k)
{
    unsigned long currentClasterId = 1;

    for (auto &currentPoint : data)
    {
        if (currentPoint.clasterId == -1)
        {
            if (isEnughtRNNToBeCorePoint(currentPoint.rnnk, k))
            {
                currentPoint.clasterId = currentClasterId;
                currentPoint.pointType = 1; //must be seed because have more rnnk;
                buildClaster(currentPoint.neighbourIndexes, data, k, currentClasterId);
                currentClasterId++;
            }
        }
    }
}

int main(int argc, char **argv)
{
    string arg1 = argv[1];
    string arg2 = argv[2];
    cout << arg1 << endl;
    cout << arg2 << endl;
    int fileNumber = stoi(arg1);
    int k = stoi(arg2);

    string *paths = new string[5];
    paths[0] = "Data/lecture.csv";
    paths[1] = "Data/dim512.csv";
    paths[2] = "Data/complex9.csv";
    paths[3] = "Data/cluto-t7-10k.csv";
    paths[4] = "Data/letter.csv";

    string *fileIdentificators = new string[5];
    fileIdentificators[0] = "lecture.csv";
    fileIdentificators[1] = "dim512.csv";
    fileIdentificators[2] = "complex9.csv";
    fileIdentificators[3] = "cluto-t7-10k.csv";
    fileIdentificators[4] = "letter.csv";

    string postIndentificator = fileIdentificators[fileNumber];
    string pathToStoreSTATfileNaive = "Data/STAT_dbscrn_naive_";
    ostringstream *oss = new ostringstream();
    *oss << pathToStoreSTATfileNaive << postIndentificator;
    STAT statFileNaive(oss->str());
    oss->str("");
    string pathToStoreSTATfileOptim = "Data/STAT_dbscrn_optim_";
    *oss << pathToStoreSTATfileOptim << postIndentificator;
    STAT statFileOptim(oss->str());
    oss->str("");

    (*oss) << "Data/OUT_dbscrn_optim_" << postIndentificator;
    string pathToOUTFileOptim = oss->str();
    oss->str("");
    (*oss) << "Data/OUT_dbscrn_naive_" << postIndentificator;
    string pathToOUTFileNaive = oss->str();
    oss->str("");

    (*oss) << "Data/DEBUG_dbscrn_optim_" << postIndentificator;
    string pathToDEBUGFileOptim = oss->str();
    oss->str("");
    (*oss) << "Data/DEBUG_dbscrn_naive_" << postIndentificator;
    string pathToDEBUGFileNaive = oss->str();
    oss->str("");

    *oss << "k = " << k;
    statFileNaive.params = oss->str();
    statFileOptim.params = oss->str();

    delete oss;

    statFileNaive.algorithm = "DBSCRN";
    statFileOptim.algorithm = "DBSCRN";

    // string pathToFile = "Data/lecture.csv";
    // string pathToFile = "Data/points.csv";
    // string pathToFile = "Data/dim512.csv";
    // string pathToFile = "Data/complex9.csv";
    // string pathToFile = "Data/cluto-t7-10k.csv";
    // string pathToFile = "Data/letter.csv";

    string pathToFile = paths[fileNumber];

    statFileOptim.nameOfInputFile = pathToFile;
    statFileNaive.nameOfInputFile = pathToFile;

    string pathToStoreTimeOfExecution = "Data/time.csv";

    TimeWriter totalTimerNaive;
    TimeWriter totalTimerOptim;
    TimeWriter timer;
    totalTimerNaive.start();
    timer.start();
    //Data access
    TimeWriter *tempTimeWirter = new TimeWriter(pathToStoreTimeOfExecution, "dataTimeLoading");
    tempTimeWirter->start();
    MDPointLoader dataReader;
    vector<MDPoint> *data = new vector<MDPoint>(dataReader.getData(pathToFile));
    vector<MDPoint> &dataR = *data;
    timer.stop();
    statFileNaive.readingDatasetTime = timer.getTime();
    statFileOptim.readingDatasetTime = timer.getTime();
    tempTimeWirter->stop();
    tempTimeWirter->writeTime();
    delete tempTimeWirter;
    //End Data access

    //Start procedure
    TimeWriter timeWriter(pathToStoreTimeOfExecution, Identyficator("dbscrn", to_string(dataR.size())));
    vector<MDPoint> dataOptim = *data;
    vector<MDPoint> &dataOptimR = dataOptim;
    NaiveRNN(dataR, k, statFileNaive.sortingDataTime);
    timer.start();
    enterToBuildClaster(dataR, k);
    timer.stop();
    statFileNaive.clasteringTime = timer.getTime();
    totalTimerNaive.stop();
    statFileNaive.totalRuntime = totalTimerNaive.getTime();

    DataWriter dataWriter;
    dataWriter.writeMDPoints(pathToDEBUGFileNaive, dataR);
    dataWriter.writeClasteringResults(pathToOUTFileNaive, dataR);
    statFileNaive.gatherDataFromCollection(dataR);
    statFileNaive.writeSTATFile();
    //end naive

    totalTimerOptim.start();
    OptimisedRNN(dataOptimR, k, statFileOptim.sortingDataTime);

    //Clasters building
    timer.start();
    enterToBuildClaster(dataOptimR, k);
    timer.stop();
    statFileOptim.clasteringTime = timer.getTime();
    // This it end and time calculation
    timeWriter.stop();
    timeWriter.writeTime();
    totalTimerOptim.stop();
    statFileOptim.totalRuntime = totalTimerOptim.getTime() + statFileOptim.readingDatasetTime;

    dataWriter.writeMDPoints(pathToDEBUGFileOptim, dataOptimR);
    //End Claster building
    dataWriter.writeClasteringResults(pathToOUTFileOptim, dataOptimR);
    statFileOptim.gatherDataFromCollection(dataOptimR);
    statFileOptim.writeSTATFile();

    cout << endl
         << "finised" << endl;
    return 0;
}