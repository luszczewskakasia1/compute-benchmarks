#pragma once

#include "framework/configuration.h"

#include <memory>
#include <string>

class Statistics {
  public:
    using Value = long long int;

    explicit Statistics(int maxSamplesCount);
    void pushValue(Value value);
    bool isEmpty();
    bool isFull();

    Value min();
    Value max();
    Value mean();
    Value median();
    double standardDeviation();

    static void printStatisticsHeader(Configuration::PrintType printType);
    void printStatistics(const std::string &testCaseName, Configuration::PrintType printType);
    void printStatisticsString(const std::string &testCaseName, Configuration::PrintType printType, const std::string &message);

  private:
    const int maxSamplesCount;
    const std::unique_ptr<Value[]> samples;
    int samplesCount = 0;
};
