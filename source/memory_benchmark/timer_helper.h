#pragma once

#include "framework/utility/statistics.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/benchmark_specific_configuration.h"

namespace TimerHelper {
inline void pushValue(Statistics &statistics, Statistics::Value timeNs, size_t bufferSize) {
    auto configuration = static_cast<BenchmarkSpecificConfiguration *>(::configuration.benchmarkSpecificConfiguration);
    if (configuration->printBandwidth) {
        statistics.pushValue(Timer::getBandwidth(timeNs, bufferSize));
    } else {
        statistics.pushValue(timeNs / 1000);
    }
}

inline void pushValue(Statistics &statistics, uint64_t timeNs, size_t bufferSize) {
    return pushValue(statistics, static_cast<Statistics::Value>(timeNs), bufferSize);
}

inline void pushValueFromTimer(Statistics &statistics, const Timer &timer, size_t bufferSize) {
    const auto timeNs = timer.getNs();
    return pushValue(statistics, timeNs, bufferSize);
}
} // namespace TimerHelper
