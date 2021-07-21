#include "framework/configuration.h"

std::string BenchmarkInfoImpl::getBenchmarkName() {
    return "multitile_memory_benchmark";
}

std::string BenchmarkInfoImpl::getBenchmarkDescription() {
    return "Multi-tile Memory Benchmark is a set of tests aimed at measuring bandwidth of memory transfers performed on a multi-tile device.";
}

int BenchmarkInfoImpl::getTestCaseNameColumnWidth() {
    return 136;
}

std::unique_ptr<BenchmarkInfoImpl::BenchmarkSpecificConfigurationBase> BenchmarkInfoImpl::createBenchmarkSpecificConfiguration(ArgumentContainer &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
