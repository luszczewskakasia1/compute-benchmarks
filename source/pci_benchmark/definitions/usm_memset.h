#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct UsmMemsetArguments : TestCaseArguments {
    MemoryPlacementTestCaseArgument memoryPlacement;
    ByteSizeTestCaseArgument bufferSize;

    UsmMemsetArguments()
        : memoryPlacement(*this, "memory"),
          bufferSize(*this, "size", "size of the buffer to be set") {}
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
