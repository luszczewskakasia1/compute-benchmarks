#pragma once

#include "framework/configuration.h"

#include <memory>
#include <string>

class Statistics {
  public:
    using Value = double;

    explicit Statistics(size_t maxSamplesCount, Configuration::PrintType printType);
    void pushValue(Value value);
    bool isEmpty();
    bool isFull();

    static void printStatisticsHeader(Configuration::PrintType printType);
    void printStatistics(const std::string &testCaseName);
    void printStatisticsString(const std::string &testCaseName, const std::string &message);

  private:
    Value min();
    Value max();
    Value mean();
    Value median();
    double standardDeviation();

    const size_t maxSamplesCount;
    const Configuration::PrintType printType;
    const std::unique_ptr<Value[]> samples;
    size_t samplesCount = 0;
};
