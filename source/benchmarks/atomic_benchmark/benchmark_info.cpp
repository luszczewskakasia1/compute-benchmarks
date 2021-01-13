#include "framework/benchmark_info.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "atomic_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Atomic Benchmark is a set of tests aimed at measuring performance of atomic operations inside kernels.";
}

BenchmarkInfoImpl::MeasurementUnit BenchmarkInfoImpl::getMeasurementUnit() {
    return BenchmarkInfoImpl::MeasurementUnit::Microseconds;
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 120;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase>{};
}
