#include "framework/benchmark_info.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "overlap_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Overlap Benchmark is a set of tests aimed at measuring how different commands benefit for simultaneous execution.";
}

BenchmarkInfoImpl::MeasurementUnit BenchmarkInfoImpl::getMeasurementUnit() {
    return BenchmarkInfoImpl::MeasurementUnit::Microseconds;
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 73;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase>{};
}
