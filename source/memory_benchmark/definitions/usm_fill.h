#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/test_case_argument_memory_placement.h"

#include <sstream>

struct UsmFillArguments : TestCaseArguments {
    MemoryPlacementTestCaseArgument memoryPlacement;
    ByteSizeTestCaseArgument bufferSize;
    ByteSizeTestCaseArgument patternSize;
    BooleanTestCaseArgument forceBlitter;
    BooleanTestCaseArgument useEvents;
    BufferContentsTestCaseArgument patternContents;

    UsmFillArguments()
        : memoryPlacement(*this, "memory"),
          bufferSize(*this, "size", "size of the buffer to be filled"),
          patternSize(*this, "patternSize"),
          forceBlitter(*this, "forceBlitter", "Force blitter engine. Test will be skipped if device does not support blitter. Warning: in OpenCL blitter may still be used even if not forced"),
          useEvents(*this, "useEvents"),
          patternContents(*this, "patternContents") {}
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
