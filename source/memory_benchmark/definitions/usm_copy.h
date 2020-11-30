#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_memory_placement.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

#include <sstream>

struct UsmCopyArguments : TestCaseArguments {
    MemoryPlacementTestCaseArgument sourcePlacement;
    MemoryPlacementTestCaseArgument destinationPlacement;
    ByteSizeTestCaseArgument size;
    BooleanTestCaseArgument forceBlitter;
    BooleanTestCaseArgument useEvents;

    UsmCopyArguments()
        : sourcePlacement(*this, "src"),
          destinationPlacement(*this, "dst"),
          size(*this, "size"),
          forceBlitter(*this, "forceBlitter", "Force blitter engine. Test will be skipped if device does not support blitter. Warning: in OpenCL blitter may still be used even if not forced"),
          useEvents(*this, "useEvents") {}
};

class UsmCopy : public TestCase<UsmCopyArguments> {
  public:
    using TestCase<UsmCopyArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates two unified shared memory buffers and measures copy bandwidth between them.";
    }

    std::string getTestCaseName() const override {
        return "UsmCopy";
    }
};
