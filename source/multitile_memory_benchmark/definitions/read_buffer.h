#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/test_case_argument/test_case_argument_device_selection.h"
#include "framework/test_case_argument/test_case_argument_multi_device_selection.h"

#include <sstream>

struct ReadBufferArguments : TestCaseArguments {
    MultiDeviceSelectionTestCaseArgument contextPlacement;
    DeviceSelectionTestCaseArgument queuePlacement;
    DeviceSelectionTestCaseArgument bufferPlacement;
    ByteSizeTestCaseArgument size;
    CompressionBooleanTestCaseArgument compressed;
    BooleanTestCaseArgument useEvents;

    ReadBufferArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device within the context will perform the copy"),
          bufferPlacement(*this, "memory", "Placement of memory for the source buffer"),
          size(*this, "size", "Size of the buffers"),
          compressed(*this, "compressed", "Select if source buffer is to be compressed."),
          useEvents(*this, "useEvents", "Select if GPU-side measurements should be done") {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, queuePlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, bufferPlacement);
    }
};

class ReadBuffer : public TestCase<ReadBufferArguments> {
  public:
    using TestCase<ReadBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures read bandwidth.";
    }

    std::string getTestCaseName() const override {
        return "ReadBuffer";
    }
};
