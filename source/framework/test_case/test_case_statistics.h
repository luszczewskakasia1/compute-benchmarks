#pragma once
#include "framework/configuration.h"
#include "framework/utility/statistics.h"

#include <memory>
#include <string>

class TestCaseStatistics : public Statistics {
  public:
    using Clock = std::chrono::high_resolution_clock;
    using Value = double;

    explicit TestCaseStatistics(size_t maxSamplesCount, Configuration::PrintType printType);

    void pushValue(Clock::duration time) override;
    void pushValue(Clock::duration time, uint64_t size) override;

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

    const Configuration::PrintType printType;
    const std::unique_ptr<Value[]> samples;
    bool reachedInfinity = false;
};
