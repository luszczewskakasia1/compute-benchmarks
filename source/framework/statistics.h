#pragma once

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
    Value standardDeviation();

    static void printStatisticsHeader();
    void printStatistics(const std::string &testCaseName);

  private:
    const int maxSamplesCount;
    const std::unique_ptr<Value[]> samples;
    int samplesCount = 0;
};
