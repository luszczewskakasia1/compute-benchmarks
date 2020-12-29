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
    Value min() const;
    Value max() const;
    Value mean() const;
    Value median() const;
    Value standardDeviation() const;

    std::string minString() const;
    std::string maxString() const;
    std::string meanString() const;
    std::string medianString() const;
    std::string standardDeviationString() const;
    static std::string generateMetricString(Value value);

    const size_t maxSamplesCount;
    const Configuration::PrintType printType;
    const std::unique_ptr<Value[]> samples;
    size_t samplesCount = 0;
    bool reachedInfinity = false;
};
