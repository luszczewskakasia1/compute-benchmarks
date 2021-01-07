#include "framework/benchmark_info.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "gpu_cmds_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Gpu Commands Benchmark is a set of tests aimed at measuring GPU-side execution duration of various commands.";
}

BenchmarkInfoImpl::MeasurementUnit BenchmarkInfoImpl::getMeasurementUnit() {
    return BenchmarkInfoImpl::MeasurementUnit::Microseconds;
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 77;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase>{};
}
