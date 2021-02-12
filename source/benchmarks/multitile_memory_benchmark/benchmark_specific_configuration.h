#pragma once

#include "framework/argument/basic_argument.h"
#include "framework/benchmark_info.h"

struct BenchmarkSpecificConfiguration : BenchmarkInfo::BenchmarkSpecificConfigurationBase {
    BooleanArgument printBandwidth;

    BenchmarkSpecificConfiguration(ArgumentContainer &testCaseArguments)
        : printBandwidth(testCaseArguments, "printBandwidth") {
        printBandwidth = true;
    }
};
