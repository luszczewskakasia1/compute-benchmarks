#pragma once
#include <chrono>

class Timer {
  public:
    using Clock = std::chrono::high_resolution_clock;

    void measureStart() {
        startTime = Clock::now();
    }

    void measureEnd() {
        endTime = Clock::now();
    }

    Clock::duration get() const {
        return endTime - startTime;
    }

  private:
    Clock::time_point startTime;
    Clock::time_point endTime;
};
