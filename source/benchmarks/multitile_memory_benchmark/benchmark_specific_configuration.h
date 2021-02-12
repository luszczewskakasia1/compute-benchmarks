#pragma once

#include "framework/benchmark_info.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct BenchmarkSpecificConfiguration : BenchmarkInfo::BenchmarkSpecificConfigurationBase {
    BooleanArgument printBandwidth;

    BenchmarkSpecificConfiguration(ArgumentsBase &testCaseArguments)
        : printBandwidth(testCaseArguments, "printBandwidth") {
        printBandwidth = true;
    }
};
