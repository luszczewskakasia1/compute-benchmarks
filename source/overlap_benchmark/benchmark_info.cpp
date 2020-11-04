#include "framework/benchmark_info.h"

std::string getBenchmarkName() {
    return "overlap_benchmark";
}

std::string getBenchmarkDescription() {
    return "Overlap Benchmark is a set of tests aimed at measuring how different commands benefit for simultaneous execution.";
}

MeasurementUnit getMeasurementUnit() {
    return MeasurementUnit::Microseconds;
}

int getTestCaseNameColumnWidth() {
    return 73;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
