#pragma once

#include "framework/utility/process.h"

#include <string>

class Statistics;

class ProcessGroup {
  public:
    ProcessGroup(const std::string &binaryName, size_t count);

    // Applying same operation for all processes
    void addArgumentAll(const std::string &key, const std::string &value);
    void addEnvVariableAll(const std::string &key, const std::string &value);
    void runAll();
    void synchronizeAll(size_t iterationsCount);
    void waitForFinishAll();
    TestResult getResultAll();

    // Operations involving all processes
    void pushMeasurementsToStatistics(size_t expectedCount, Statistics &statistics, bool pushIndividualProcessesMeasurements, bool pushAveragedMeasurements);

    // Container-like methods
    Process &operator[](size_t index);
    size_t size() const;

  private:
    const std::string binaryName;
    std::vector<Process> processes = {};
};
