#include "naiveReverseNearesNeighbour.h"
#include "timeMeasure.h"

void NaiveRNN(vector<MDPoint> &data)
{
  string pathToStoreTimeOfExecution = "Data/time.csv";
  TimeWriter timeWriter(pathToStoreTimeOfExecution, "calcNNk , pure version");
  timeWriter.start();
  for (auto i : data)
  {
    i.calcNNk(3, data);
  }
  timeWriter.stop();
  timeWriter.writeTime();
}