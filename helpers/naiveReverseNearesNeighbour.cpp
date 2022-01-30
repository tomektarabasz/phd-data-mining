#include "naiveReverseNearesNeighbour.h"
#include "timeMeasure.h"

void NaiveRNN(vector<MDPoint> &data, int k, double& executionTime)
{
  string pathToStoreTimeOfExecution = "Data/time.csv";
  TimeWriter timeWriter(pathToStoreTimeOfExecution, "calcNNk , pure version");
  timeWriter.start();
  TimeWriter timer;
  size_t index = 0;
  for (auto &i : data)
  {
    timer.start();
    i.calcNNk(k, data, index, executionTime);
    timer.stop();
    index++;
    i.timeToFindNeighbour = timer.getTime(); 
  }
  timeWriter.stop();
  timeWriter.writeTime();
  
}