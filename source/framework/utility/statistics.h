#pragma once

#include "framework/configuration.h"

#include <chrono>
#include <memory>
#include <string>

class Statistics {
  public:
    using Clock = std::chrono::high_resolution_clock;
    using Value = double;

    explicit Statistics(size_t maxSamplesCount, Configuration::PrintType printType);

    void pushValue(Clock::duration time);
    void pushValue(Clock::duration time, uint64_t size);

    bool isEmpty();
    bool isFull();

    static void printStatisticsHeader(Configuration::PrintType printType);
    void printStatistics(const std::string &testCaseName);
    void printStatisticsString(const std::string &testCaseName, const std::string &message);

  private:
    void pushValue(Value value);
    Value min();
    Value max();
    Value mean();
    Value median();
    double standardDeviation();

    const size_t maxSamplesCount;
    const Configuration::PrintType printType;
    const std::unique_ptr<Value[]> samples;
    size_t samplesCount = 0;
    bool reachedInfinity = false;
};
