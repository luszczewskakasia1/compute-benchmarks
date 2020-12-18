#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_usm_initial_placement.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

#include <sstream>

struct UsmSharedFirstCpuAccessArguments : TestCaseArguments {
    UsmInitialPlacementTestCaseArgument initialPlacement;
    ByteSizeTestCaseArgument bufferSize;

    UsmSharedFirstCpuAccessArguments()
        : initialPlacement(*this, "initialPlacement", "Hint for initial placement of the resource passed to the driver"),
          bufferSize(*this, "size", "Size of the buffer") {}
};

struct UsmSharedFirstCpuAccess : TestCase<UsmSharedFirstCpuAccessArguments> {
    using TestCase<UsmSharedFirstCpuAccessArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmSharedFirstCpuAccess";
    }

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to access it on CPU after creation.";
    }
};
