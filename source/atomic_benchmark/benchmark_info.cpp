#include "framework/benchmark_info.h"

std::string getBenchmarkName() {
    return "atomic_benchmark";
}

std::string getBenchmarkDescription() {
    return "Atomic Benchmark is a set of tests aimed at measuring performance of atomic operations inside kernels.";
}

MeasurementUnit getMeasurementUnit() {
    return MeasurementUnit::Microseconds;
}

int getTestCaseNameColumnWidth() {
    return 47;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
