#include "timeMeasure.h"
#include "ctime"

TimeWriter::TimeWriter(string path, string identifire) : pathToFile(path), identifire(identifire){};
TimeWriter::TimeWriter(){};

void TimeWriter::start()
{

    using std::chrono::high_resolution_clock;
    this->startTime = high_resolution_clock::now();
}

void TimeWriter::stop()
{

    using std::chrono::high_resolution_clock;
    this->stopTime = high_resolution_clock::now();
}

double TimeWriter::getTime(){
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;
    duration<double, std::milli> ms_double = this->stopTime - this->startTime;
    return ms_double.count();
}

void TimeWriter::writeTime()
{

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;
    duration<double, std::milli> ms_double = this->stopTime - this->startTime;
    ofstream timeFile(this->pathToFile, std::ios_base::app);
    if (timeFile.is_open())
    {
        time_t rawtime;
        struct tm *timeinfo;
        char buffer[80];

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
        std::string dateOfExperiment(buffer);
        timeFile << dateOfExperiment << "," << ms_double.count() << "," << identifire << endl;
        timeFile.close();
    }
    else
        cout << "Unable to open file";
}
