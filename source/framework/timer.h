#pragma once
#include <chrono>

class Timer {
  public:
    inline void measureStart() {
        *((std::chrono::high_resolution_clock::time_point *)&m_startTime) =
            std::chrono::high_resolution_clock::now();
    }

    inline void measureEnd() {
        *((std::chrono::high_resolution_clock::time_point *)&m_endTime) =
            std::chrono::high_resolution_clock::now();
    }

    long long int Get() {
        long long int nanosecondTime = 0;
        std::chrono::duration<double> diffTime = std::chrono::duration_cast<
            std::chrono::duration<double>>(
            *(reinterpret_cast<std::chrono::high_resolution_clock::time_point *>(
                &m_endTime)) -
            *(reinterpret_cast<std::chrono::high_resolution_clock::time_point *>(
                &m_startTime)));
        nanosecondTime = (long long int)(diffTime.count() * (double)1000000000.0);
        return nanosecondTime;
    }

    long long GetStart() {
        long long ret =
            (long long)(reinterpret_cast<
                            std::chrono::high_resolution_clock::time_point *>(
                            &m_startTime)
                            ->time_since_epoch()
                            .count());
        return ret;
    }

    long long GetEnd() {
        long long ret =
            (long long)(reinterpret_cast<
                            std::chrono::high_resolution_clock::time_point *>(
                            &m_endTime)
                            ->time_since_epoch()
                            .count());
        return ret;
    }

    // This operator enables setting the same start time for more Timers
    Timer &operator=(const Timer &t) {
        m_startTime = t.m_startTime;
        return *this;
    }

    static void SetFreq() {}

  private:
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_endTime;
};
