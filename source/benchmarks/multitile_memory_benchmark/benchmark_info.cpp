#include "framework/configuration.h"

#include "benchmark_specific_configuration.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "multitile_memory_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Multi-tile Memory Benchmark is a set of tests aimed at measuring bandwidth of memory transfers performed on a multi-tile device.";
}

MeasurementUnit BenchmarkInfoImpl::getMeasurementUnit() {
    auto configuration = static_cast<BenchmarkSpecificConfiguration *>(Configuration::get().benchmarkSpecificConfiguration);
    if (configuration->printBandwidth) {
        return MeasurementUnit::GigabytesPerSecond;
    } else {
        return MeasurementUnit::Microseconds;
    }
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 136;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(ArgumentContainer &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase>{new BenchmarkSpecificConfiguration(testCaseArguments)};
}
