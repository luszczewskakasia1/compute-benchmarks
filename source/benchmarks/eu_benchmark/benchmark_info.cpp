#include "framework/benchmark_info.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "eu_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "EU Benchmark is a set of tests aimed at measuring performance of calculations performed in kernels.";
}

MeasurementUnit BenchmarkInfoImpl::getMeasurementUnit() {
    return MeasurementUnit::Microseconds;
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 61;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(ArgumentContainer &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase>{};
}
