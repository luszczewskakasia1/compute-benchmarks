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

    long long int Get() const {
        std::chrono::duration<double> diffTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
        long long int nanosecondTime = (long long int)(diffTime.count() * (double)1000000000.0);
        return nanosecondTime;
    }

  private:
    Clock::time_point startTime;
    Clock::time_point endTime;
};
