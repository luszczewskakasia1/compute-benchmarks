#include "memory_benchmark/benchmark_specific_configuration.h"

std::string getBenchmarkName() {
    return "multitile_memory_benchmark";
}

std::string getBenchmarkDescription() {
    return "Multi-tile Memory Benchmark is a set of tests aimed at measuring bandwidth of memory transfers performed on a multi-tile device.";
}

MeasurementUnit getMeasurementUnit() {
    auto configuration = static_cast<BenchmarkSpecificConfiguration *>(::configuration.benchmarkSpecificConfiguration);
    if (configuration->printBandwidth) {
        return MeasurementUnit::GigabytesPerSecond;
    } else {
        return MeasurementUnit::Microseconds;
    }
}

int getTestCaseNameColumnWidth() {
    return 133;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{new BenchmarkSpecificConfiguration(testCaseArguments)};
}
