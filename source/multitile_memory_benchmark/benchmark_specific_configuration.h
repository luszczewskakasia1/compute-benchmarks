#pragma once

#include "framework/benchmark_info.h"

struct BenchmarkSpecificConfiguration : BenchmarkSpecificConfigurationBase {
    BooleanTestCaseArgument printBandwidth;

    BenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments)
        : printBandwidth(testCaseArguments, "printBandwidth") {
        printBandwidth = true;
    }
};
