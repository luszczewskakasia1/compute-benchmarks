#include "framework/benchmark_info.h"

std::string BenchmarkInfo::getBenchmarkName() {
    return "ulls_benchmark";
}

std::string BenchmarkInfo::getBenchmarkDescription() {
    return "Ulls Benchmark is a set of tests aimed at measuring Ultra Low Latency Submission (ULLS) performance impact.";
}

BenchmarkInfo::MeasurementUnit BenchmarkInfo::getMeasurementUnit() {
    return BenchmarkInfo::MeasurementUnit::Microseconds;
}

int BenchmarkInfo::getTestCaseNameColumnWidth() {
    return 67;
}

std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase> BenchmarkInfo::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
