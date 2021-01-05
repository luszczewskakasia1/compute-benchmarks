#pragma once

#include "framework/benchmark_info.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct BenchmarkSpecificConfiguration : BenchmarkInfo::BenchmarkSpecificConfigurationBase {
    BooleanTestCaseArgument printBandwidth;

    BenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments)
        : printBandwidth(testCaseArguments, "printBandwidth") {
        printBandwidth = true;
    }
};
