#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/test_case_argument/test_case_argument_device_selection.h"
#include "framework/test_case_argument/test_case_argument_multi_device_selection.h"

#include <sstream>

struct CopyBufferArguments : TestCaseArguments {
    MultiDeviceSelectionTestCaseArgument contextPlacement;
    DeviceSelectionTestCaseArgument queuePlacement;
    DeviceSelectionTestCaseArgument srcPlacement;
    DeviceSelectionTestCaseArgument dstPlacement;
    ByteSizeTestCaseArgument size;
    CompressionBooleanTestCaseArgument srcCompressed;
    CompressionBooleanTestCaseArgument dstCompressed;

    BooleanTestCaseArgument useEvents;

    CopyBufferArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device within the context will perform the copy"),
          srcPlacement(*this, "src", "Placement of memory for the source buffer"),
          dstPlacement(*this, "dst", "Placement of memory for the destination buffer"),
          size(*this, "size", "Size of the buffers"),
          srcCompressed(*this, "srcCompressed", "Select if source buffer is to be compressed."),
          dstCompressed(*this, "dstCompressed", "Select if destination buffer is to be compressed."),
          useEvents(*this, "useEvents", "Select if GPU-side measurements should be done") {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, queuePlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, srcPlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, dstPlacement);
    }
};

class CopyBuffer : public TestCase<CopyBufferArguments> {
  public:
    using TestCase<CopyBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates two OpenCL buffers and measures copy bandwidth between them.";
    }

    std::string getTestCaseName() const override {
        return "CopyBuffer";
    }
};
