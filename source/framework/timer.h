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

    double Get() const {
        std::chrono::duration<double> diffTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
        double microsecondTime = diffTime.count() * 1000000.0;
        return microsecondTime;
    }

    double getBandwidth(size_t bufferSizeInBytes) const {
        const auto timeNs = Get() * 1000.0;
        return bufferSizeInBytes / timeNs; // Bytes/Nanoseconds = Gigabytes/Seconds
    }

  private:
    Clock::time_point startTime;
    Clock::time_point endTime;
};
