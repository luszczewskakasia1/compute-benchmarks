#pragma once

#include "framework/utility/error.h"

#include <chrono>

class Statistics {
  public:
    using Clock = std::chrono::high_resolution_clock;

    Statistics(size_t maxSamplesCount) : maxSamplesCount(maxSamplesCount) {}

    virtual void pushValue(Clock::duration time) = 0;
    virtual void pushValue(Clock::duration time, uint64_t size) = 0;

    bool isEmpty() const { return samplesCount == 0; }
    bool isFull() const { return samplesCount == maxSamplesCount; }

  protected:
    void incrementSamplesCount() {
        ERROR_IF(samplesCount > maxSamplesCount, "Too much values pushed by the test");
        samplesCount++;
    }
    const size_t maxSamplesCount = 0;
    size_t samplesCount = 0;
};
