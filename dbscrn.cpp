#include <string>
#include "models/mdPoint.h"
#include <vector>
#include "preprocessing/mdPointLoader.h"
#include "helpers/timeMeasure.h"
#include "helpers/identyficator.h"

using namespace std;

int main(){
    string pathToFile = "Data/lecture.csv";
    string pathToStoreTimeOfExecution = "Data/time.csv";

    //Data access
    TimeWriter* tempTimeWirter = new TimeWriter(pathToStoreTimeOfExecution,"dataTimeLoading");
    tempTimeWirter->start();
    MDPointLoader dataReader;
    vector<MDPoint> *data = new vector<MDPoint>(dataReader.getData(pathToFile));
    vector<MDPoint> &dataR = *data;
    tempTimeWirter->stop();
    tempTimeWirter->writeTime();
    delete tempTimeWirter;
    //End Data access

    //Start procedure
    TimeWriter timeWriter(pathToStoreTimeOfExecution, Identyficator("dbscrn",to_string(dataR.size())));
    timeWriter.start();

    MDPoint point = dataR[0];
    point.calcNNk(3,dataR);

    // This it end and time calculation
    timeWriter.stop();
    timeWriter.writeTime();

    return 0;
}