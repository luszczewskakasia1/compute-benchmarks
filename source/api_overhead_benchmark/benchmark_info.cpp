#include "framework/benchmark_info.h"

std::string getBenchmarkName() {
    return "api_overhead_benchmark";
}

std::string getBenchmarkDescription() {
    return "Api Overhead Benchmark is a set of tests aimed at measuring CPU-side execution duration of compute API calls.";
}

MeasurementUnit getMeasurementUnit() {
    return MeasurementUnit::Microseconds;
}

int getTestCaseNameColumnWidth() {
    return 68;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
