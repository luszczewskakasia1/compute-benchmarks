#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/test_case_argument_long_hex.h"
#include "framework/test_case_argument/test_case_argument_memory_placement.h"

#include <sstream>

struct UsmFillSpecificPatternArguments : TestCaseArguments {
    MemoryPlacementTestCaseArgument memoryPlacement;
    ByteSizeTestCaseArgument bufferSize;
    LongHexTestCaseArgument pattern;
    BooleanTestCaseArgument forceBlitter;
    BooleanTestCaseArgument useEvents;

    UsmFillSpecificPatternArguments()
        : memoryPlacement(*this, "memory"),
          bufferSize(*this, "size", "size of the buffer to be filled"),
          pattern(*this, "pattern"),
          forceBlitter(*this, "forceBlitter", "Force blitter engine. Test will be skipped if device does not support blitter. Warning: in OpenCL blitter may still be used even if not forced"),
          useEvents(*this, "useEvents") {}
};

class UsmFillSpecificPattern : public TestCase<UsmFillSpecificPatternArguments> {
  public:
    using TestCase<UsmFillSpecificPatternArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a unified memory buffer and measures fill bandwidth. Allow specifying arbitrary pattern.";
    }

    std::string getTestCaseName() const override {
        return "UsmFillSpecificPattern";
    }
};
