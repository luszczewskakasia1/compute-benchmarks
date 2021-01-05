#include "framework/configuration.h"
#include "multitile_memory_benchmark/benchmark_specific_configuration.h"

std::string BenchmarkInfo::getBenchmarkName() {
    return "multitile_memory_benchmark";
}

std::string BenchmarkInfo::getBenchmarkDescription() {
    return "Multi-tile Memory Benchmark is a set of tests aimed at measuring bandwidth of memory transfers performed on a multi-tile device.";
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
    return 136;
}

std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase> BenchmarkInfo::createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfo::BenchmarkSpecificConfigurationBase>{new BenchmarkSpecificConfiguration(testCaseArguments)};
}
