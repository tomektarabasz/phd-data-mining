#include "naiveReverseNearesNeighbour.h"
#include "timeMeasure.h"

void NaiveRNN(vector<MDPoint> &data, int k)
{
  string pathToStoreTimeOfExecution = "Data/time.csv";
  TimeWriter timeWriter(pathToStoreTimeOfExecution, "calcNNk , pure version");
  timeWriter.start();
  size_t index = 0;
  for (auto &i : data)
  {
    i.calcNNk(k, data, index);
    index++;
  }
  timeWriter.stop();
  timeWriter.writeTime();
}