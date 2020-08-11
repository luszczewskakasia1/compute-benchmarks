#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct UsmMemsetArguments : TestCaseArguments {
    MemoryPlacementTestCaseArgument memoryPlacement;
    PositiveIntegerTestCaseArgument bufferSize;

    UsmMemsetArguments()
        : memoryPlacement("memory", "host or device"),
          bufferSize("size", "size of the buffer to be filled") {
        arguments.push_back(&memoryPlacement);
        arguments.push_back(&bufferSize);
    }
};

class UsmMemset : public TestCase<UsmMemsetArguments> {
  public:
    using TestCase<UsmMemsetArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a unified memory buffer and measures memset bandwidth";
    }

    std::string getTestCaseName() const override {
        return "UsmMemset";
    }
};
