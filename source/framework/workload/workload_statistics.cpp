#include "workload_statistics.h"

#include "framework/utility/error.h"

#include <iostream>

void WorkloadStatistics::pushValue(Clock::duration time) {
    incrementSamplesCount();
    const auto timeNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    result << timeNanoseconds << ' ';
}

void WorkloadStatistics::pushValue(Clock::duration time, uint64_t size) {
    ERROR("Not implemented");
}

void WorkloadStatistics::printStatistics() {
    std::cout << result.str() << std::endl;
}
