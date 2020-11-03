#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/test_case_argument/test_case_argument_device_selection.h"
#include "framework/test_case_argument/test_case_argument_multi_device_selection.h"

#include <sstream>

struct FillBufferArguments : TestCaseArguments {
    MultiDeviceSelectionTestCaseArgument contextPlacement;
    DeviceSelectionTestCaseArgument queuePlacement;
    DeviceSelectionTestCaseArgument bufferPlacement;
    ByteSizeTestCaseArgument size;
    ByteSizeTestCaseArgument patternSize;
    CompressionBooleanTestCaseArgument compressed;
    BooleanTestCaseArgument copyQueue;

    FillBufferArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device will perform the fill"),
          bufferPlacement(*this, "memory", "Placement of memory for the buffer"),
          size(*this, "size", "Size of the buffer"),
          patternSize(*this, "patternSize", "Size of the fill pattern"),
          compressed(*this, "compressed", "Select if the buffer will be compressed"),
          copyQueue(*this, "copyQueue", "Force blitter") {}
};

class FillBuffer : public TestCase<FillBufferArguments> {
  public:
    using TestCase<FillBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures fill bandwidth.";
    }

    std::string getTestCaseName() const override {
        return "FillBuffer";
    }
};
