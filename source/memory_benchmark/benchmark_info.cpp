#include "framework/configuration.h"
#include "memory_benchmark/benchmark_specific_configuration.h"

std::string BenchmarkInfo::getBenchmarkName() {
    return "memory_benchmark";
}

std::string BenchmarkInfo::getBenchmarkDescription() {
    return "Memory Benchmark is a set of tests aimed at measuring bandwidth of memory transfers.";
}

BenchmarkInfo::MeasurementUnit BenchmarkInfo::getMeasurementUnit() {
    auto configuration = static_cast<BenchmarkSpecificConfiguration *>(::configuration.benchmarkSpecificConfiguration);
    if (configuration->printBandwidth) {
        return BenchmarkInfo::MeasurementUnit::GigabytesPerSecond;
    } else {
        return BenchmarkInfo::MeasurementUnit::Microseconds;
    }
}

int BenchmarkInfo::getTestCaseNameColumnWidth() {
    return 121;
}

std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase> BenchmarkInfo::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase>{new BenchmarkSpecificConfiguration(testCaseArguments)};
}
