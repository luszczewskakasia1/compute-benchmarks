#include "framework/benchmark_info.h"

std::string BenchmarkInfo::getBenchmarkName() {
    return "atomic_benchmark";
}

std::string BenchmarkInfo::getBenchmarkDescription() {
    return "Atomic Benchmark is a set of tests aimed at measuring performance of atomic operations inside kernels.";
}

BenchmarkInfo::MeasurementUnit BenchmarkInfo::getMeasurementUnit() {
    return BenchmarkInfo::MeasurementUnit::Microseconds;
}

int BenchmarkInfo::getTestCaseNameColumnWidth() {
    return 120;
}

std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase> BenchmarkInfo::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase>{};
}
