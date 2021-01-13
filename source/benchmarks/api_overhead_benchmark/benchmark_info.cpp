#include "framework/benchmark_info.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "api_overhead_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Api Overhead Benchmark is a set of tests aimed at measuring CPU-side execution duration of compute API calls.";
}

BenchmarkInfoImpl::MeasurementUnit BenchmarkInfoImpl::getMeasurementUnit() {
    return BenchmarkInfoImpl::MeasurementUnit::Microseconds;
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 68;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase>{};
}
