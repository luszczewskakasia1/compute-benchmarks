#pragma once

#include <chrono>
#include <sstream>

class WorkloadStatistics {
  public:
    using Clock = std::chrono::high_resolution_clock;

    void pushValue(Clock::duration time);
    void printStatistics();

  private:
    std::ostringstream result = {};
};
