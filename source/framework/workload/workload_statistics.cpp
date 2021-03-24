#include "workload_statistics.h"

#include "framework/utility/error.h"
#include "framework/workload/workload_io.h"

#include <iostream>

void WorkloadStatistics::pushValue(Clock::duration time, const std::string &description, MeasurementUnit unit) {
    FATAL_ERROR_IF(description != "", "WorkloadStatistics does not support multiple statistics groups");
    FATAL_ERROR_IF(samplesCount == maxSamplesCount, "Too many values pushed by the test");
    samplesCount++;

    const auto timeNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    result << timeNanoseconds << ' ';
}

void WorkloadStatistics::pushValue(Clock::duration time, uint64_t size, const std::string &description, MeasurementUnit unit) {
    FATAL_ERROR_IF(description != "", "WorkloadStatistics does not support multiple statistics groups");
    FATAL_ERROR("Not implemented");
}

bool WorkloadStatistics::isEmpty() const {
    return samplesCount == 0;
}

bool WorkloadStatistics::isFull() const {
    return samplesCount == maxSamplesCount;
}

void WorkloadStatistics::printStatistics(WorkloadIo &io) {
    io.writeToMeasurements(result.str());
}
