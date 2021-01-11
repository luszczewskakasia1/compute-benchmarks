#pragma once

#include "framework/utility/statistics.h"

#include <chrono>
#include <sstream>

class WorkloadStatistics : public Statistics {
  public:
    using Statistics::Statistics;
    using Clock = std::chrono::high_resolution_clock;

    void printStatistics();

    void pushValue(Clock::duration time) override;
    void pushValue(Clock::duration time, uint64_t size) override;

  private:
    std::ostringstream result = {};
    size_t samplesCount = 0;
};
