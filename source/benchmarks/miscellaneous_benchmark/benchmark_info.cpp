#include "framework/benchmark_info.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "miscellaneous_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Miscellaneous Benchmark is a set of tests measuring different simple compute scenarios.";
}

MeasurementUnit BenchmarkInfoImpl::getMeasurementUnit() {
    return MeasurementUnit::Microseconds;
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 90;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(ArgumentContainer &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
