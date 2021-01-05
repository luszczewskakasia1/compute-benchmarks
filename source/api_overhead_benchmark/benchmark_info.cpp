#include "framework/benchmark_info.h"

std::string BenchmarkInfo::getBenchmarkName() {
    return "api_overhead_benchmark";
}

std::string BenchmarkInfo::getBenchmarkDescription() {
    return "Api Overhead Benchmark is a set of tests aimed at measuring CPU-side execution duration of compute API calls.";
}

BenchmarkInfo::MeasurementUnit BenchmarkInfo::getMeasurementUnit() {
    return BenchmarkInfo::MeasurementUnit::Microseconds;
}

int BenchmarkInfo::getTestCaseNameColumnWidth() {
    return 68;
}

std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase> BenchmarkInfo::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase>{};
}
