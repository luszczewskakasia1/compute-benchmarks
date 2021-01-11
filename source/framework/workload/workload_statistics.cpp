#include "workload_statistics.h"

#include <iostream>

void WorkloadStatistics::pushValue(Clock::duration time) {
    const auto timeNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    result << timeNanoseconds << ' ';
}

void WorkloadStatistics::printStatistics() {
    std::cout << result.str() << std::endl;
}
