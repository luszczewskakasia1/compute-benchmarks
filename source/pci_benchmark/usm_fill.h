#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct UsmFillArguments : TestCaseArguments {
    MemoryPlacementTestCaseArgument memoryPlacement;
    PositiveIntegerTestCaseArgument bufferSize;
    PositiveIntegerTestCaseArgument patternSize;

    UsmFillArguments()
        : memoryPlacement("memory", "host or device"),
          bufferSize("size", "size of the buffer to be filled"),
          patternSize("patternSize") {
        arguments.push_back(&memoryPlacement);
        arguments.push_back(&bufferSize);
        arguments.push_back(&patternSize);
    }
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
