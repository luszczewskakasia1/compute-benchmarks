#pragma once

#include "framework/utility/statistics.h"

#include <chrono>
#include <sstream>

class WorkloadStatistics : public Statistics {
  public:
    using Clock = std::chrono::high_resolution_clock;

    void printStatistics();

    void pushValue(Clock::duration time) override;
    void pushValue(Clock::duration time, uint64_t size) override;



  private:
    std::ostringstream result = {};
    size_t samplesCount = 0;
};

/*

    virtual void pushValue(Clock::duration time) = 0;
    virtual void pushValue(Clock::duration time, uint64_t size) = 0;

    virtual bool isEmpty() const = 0;
    virtual bool isFull() const = 0;

*/
