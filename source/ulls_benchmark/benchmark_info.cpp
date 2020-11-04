#include "framework/benchmark_info.h"

std::string getBenchmarkName() {
    return "ulls_benchmark";
}

std::string getBenchmarkDescription() {
    return "Ulls Benchmark is a set of tests aimed at measuring Ultra Low Latency Submission (ULLS) performance impact.";
}

MeasurementUnit getMeasurementUnit() {
    return MeasurementUnit::Microseconds;
}

int getTestCaseNameColumnWidth() {
    return 60;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
