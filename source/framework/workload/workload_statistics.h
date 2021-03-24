#pragma once

#include "framework/utility/statistics.h"

#include <chrono>
#include <sstream>

class WorkloadIo;

class WorkloadStatistics : public Statistics {
  public:
    using Statistics::Statistics;
    using Clock = std::chrono::high_resolution_clock;

    void printStatistics(WorkloadIo &io);

    void pushValue(Clock::duration time, const std::string &description = "", MeasurementUnit unit = MeasurementUnit::Default) override;
    void pushValue(Clock::duration time, uint64_t size, const std::string &description = "", MeasurementUnit unit = MeasurementUnit::Default) override;

    bool isEmpty() const override;
    bool isFull() const override;

  private:
    std::ostringstream result{};
    size_t samplesCount = 0;
};
