#include "framework/benchmark_info.h"

std::string BenchmarkInfo::getBenchmarkName() {
    return "overlap_benchmark";
}

std::string BenchmarkInfo::getBenchmarkDescription() {
    return "Overlap Benchmark is a set of tests aimed at measuring how different commands benefit for simultaneous execution.";
}

BenchmarkInfo::MeasurementUnit BenchmarkInfo::getMeasurementUnit() {
    return BenchmarkInfo::MeasurementUnit::Microseconds;
}

int BenchmarkInfo::getTestCaseNameColumnWidth() {
    return 73;
}

std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase> BenchmarkInfo::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase>{};
}
