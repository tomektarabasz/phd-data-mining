#include <string>
#include "models/mdPoint.h"
#include <vector>
#include "preprocessing/mdPointLoader.h"
#include "helpers/timeMeasure.h"
#include "helpers/identyficator.h"
#include "helpers/naiveReverseNearesNeighbour.h"
#include "helpers/optimisedReverseNearesNeighbour.h"
#include "./preprocessing/dataWriter.h"

using namespace std;

int main()
{
    // string pathToFile = "Data/lecture.csv";
    // string pathToFile = "Data/points.csv";
    // string pathToFile = "Data/dim512.csv";
    // string pathToFile = "Data/complex9.csv";
    // string pathToFile = "Data/cluto-t7-10k.csv";
    string pathToFile = "Data/letter.csv";

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
    string pathToResultFile = "Data/naiveNNk.csv";
    dataWriter.writeMDPoints(pathToResultFile, dataR);

    OptimisedRNN(dataOptimR, k);
    pathToResultFile = "Data/optimNNk.csv";
    dataWriter.writeMDPoints(pathToResultFile, dataOptimR);

    // This it end and time calculation
    timeWriter.stop();
    timeWriter.writeTime();

    return 0;
}