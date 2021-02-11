#pragma once

#include <string>

class WorkloadIo {
  public:
    void writeToConsole(const std::string &message);
    void writeToMeasurements(uint64_t measurement);
    void writeSynchronizationChar(char c);
    char readSynchronizationChar();
};
