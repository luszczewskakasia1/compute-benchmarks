#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_memory_placement.h"

#include <sstream>

struct UsmFillArguments : TestCaseArguments {
    MemoryPlacementTestCaseArgument memoryPlacement;
    ByteSizeTestCaseArgument bufferSize;
    ByteSizeTestCaseArgument patternSize;

    UsmFillArguments()
        : memoryPlacement(*this, "memory"),
          bufferSize(*this, "size", "size of the buffer to be filled"),
          patternSize(*this, "patternSize") {}
};

class UsmFill : public TestCase<UsmFillArguments> {
  public:
    using TestCase<UsmFillArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a unified memory buffer and measures fill bandwidth";
    }

    std::string getTestCaseName() const override {
        return "UsmFill";
    }
};
