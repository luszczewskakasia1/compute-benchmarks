#include "framework/benchmark_info.h"

std::string getBenchmarkName() {
    return "multitile_memory_benchmark";
}

std::string getBenchmarkDescription() {
    return "Multi-tile Memory Benchmark is a set of tests aimed at measuring bandwidth of memory transfers performed on a multi-tile device.";
}

std::string getMeasurmentsUnit() {
    return "GB/s";
}

int getTestCaseNameColumnWidth() {
    return 106;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
