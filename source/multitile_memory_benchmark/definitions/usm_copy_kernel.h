#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_device_selection.h"
#include "framework/test_case_argument/enum/test_case_argument_multi_device_selection.h"
#include "framework/test_case_argument/enum/test_case_argument_usm_device_selection.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

#include <sstream>

struct UsmCopyKernelArguments : TestCaseArguments {
    MultiDeviceSelectionTestCaseArgument contextPlacement;
    DeviceSelectionTestCaseArgument queuePlacement;
    UsmDeviceSelectionTestCaseArgument srcPlacement;
    UsmDeviceSelectionTestCaseArgument dstPlacement;
    ByteSizeTestCaseArgument size;
    BooleanTestCaseArgument forceBlitter;
    BooleanTestCaseArgument useEvents;

    UsmCopyKernelArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device within the context will perform the operation"),
          srcPlacement(*this, "src", "Placement of memory for the source buffer"),
          dstPlacement(*this, "dst", "Placement of memory for the destination buffer"),
          size(*this, "size", "Size of the buffers"),
          forceBlitter(*this, "forceBlitter", "Force blitter engine. Test will be skipped if device does not support blitter. Warning: in OpenCL blitter may still be used even if not forced"),
          useEvents(*this, "useEvents", "Select if GPU-side measurements should be done") {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, queuePlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(srcPlacement)) &&
               DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(dstPlacement));
    }
};

class UsmCopyKernel : public TestCase<UsmCopyKernelArguments> {
  public:
    using TestCase<UsmCopyKernelArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates two unified shared memory buffers and measures copy bandwidth between them using a custom kernel.";
    }

    std::string getTestCaseName() const override {
        return "UsmCopyKernel";
    }
};
