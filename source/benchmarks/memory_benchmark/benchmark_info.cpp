#include "framework/configuration.h"

#include "benchmark_specific_configuration.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "memory_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Memory Benchmark is a set of tests aimed at measuring bandwidth of memory transfers.";
}

BenchmarkInfoImpl::MeasurementUnit BenchmarkInfoImpl::getMeasurementUnit() {
    auto configuration = static_cast<BenchmarkSpecificConfiguration *>(Configuration::get().benchmarkSpecificConfiguration);
    if (configuration->printBandwidth) {
        return BenchmarkInfoImpl::MeasurementUnit::GigabytesPerSecond;
    } else {
        return BenchmarkInfoImpl::MeasurementUnit::Microseconds;
    }
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 121;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(ArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase>{new BenchmarkSpecificConfiguration(testCaseArguments)};
}
