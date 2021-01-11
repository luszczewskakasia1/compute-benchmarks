#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_multi_device_selection.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct MultiProcessComputeArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument bufferSize;
    MultipleTilesSelectionTestCaseArgument deviceSelection;

    MultiProcessComputeArguments()
        : bufferSize(*this, "size", "Size of the buffer"),
          deviceSelection(*this, "tiles", "Tiles for execution") {}
};

struct MultiProcessCompute : TestCase<MultiProcessComputeArguments> {
    using TestCase<MultiProcessComputeArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "MultiProcessCompute";
    }

    std::string getHelp() const override {
        return "Creates a separate process for each tile specified performing a compute workload "
               "and measures average time to complete all of them. Processes will use affinity "
               "mask to select specific sub-devices for the execution";
    }
};
