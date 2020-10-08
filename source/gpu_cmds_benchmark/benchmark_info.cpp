#include "framework/benchmark_info.h"

std::string getBenchmarkName() {
    return "gpu_cmds_benchmark";
}

std::string getBenchmarkDescription() {
    return "Gpu Commands Benchmark is a set of tests aimed at measuring GPU-side execution duration of various commands.";
}

std::string getMeasurmentsUnit() {
    return "us";
}

int getTestCaseNameColumnWidth() {
    return 77;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
