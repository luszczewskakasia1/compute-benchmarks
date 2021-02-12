#include "framework/benchmark_info.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "ulls_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Ulls Benchmark is a set of tests aimed at measuring Ultra Low Latency Submission (ULLS) performance impact.";
}

BenchmarkInfoImpl::MeasurementUnit BenchmarkInfoImpl::getMeasurementUnit() {
    return BenchmarkInfoImpl::MeasurementUnit::Microseconds;
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 67;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(ArgumentContainer &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
