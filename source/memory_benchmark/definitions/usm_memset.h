#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_memory_placement.h"

#include <sstream>

struct UsmMemsetArguments : TestCaseArguments {
    MemoryPlacementTestCaseArgument memoryPlacement;
    ByteSizeTestCaseArgument bufferSize;
    BooleanTestCaseArgument copyQueue;

    UsmMemsetArguments()
        : memoryPlacement(*this, "memory"),
          bufferSize(*this, "size", "size of the buffer to be set"),
          copyQueue(*this, "copyQueue") {}
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
