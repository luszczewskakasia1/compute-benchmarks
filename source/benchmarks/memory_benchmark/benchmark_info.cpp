#include "framework/configuration.h"

#include "benchmark_specific_configuration.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "memory_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Memory Benchmark is a set of tests aimed at measuring bandwidth of memory transfers.";
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 124;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(ArgumentContainer &testCaseArguments) {
    return std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase>{new BenchmarkSpecificConfiguration(testCaseArguments)};
}
