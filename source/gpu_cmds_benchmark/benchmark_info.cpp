#include "framework/benchmark_info.h"

std::string BenchmarkInfo::getBenchmarkName() {
    return "gpu_cmds_benchmark";
}

std::string BenchmarkInfo::getBenchmarkDescription() {
    return "Gpu Commands Benchmark is a set of tests aimed at measuring GPU-side execution duration of various commands.";
}

BenchmarkInfo::MeasurementUnit BenchmarkInfo::getMeasurementUnit() {
    return BenchmarkInfo::MeasurementUnit::Microseconds;
}

int BenchmarkInfo::getTestCaseNameColumnWidth() {
    return 77;
}

std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase> BenchmarkInfo::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase>{};
}
