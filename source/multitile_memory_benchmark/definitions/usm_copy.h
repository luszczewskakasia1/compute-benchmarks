#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/test_case_argument/test_case_argument_device_selection.h"
#include "framework/test_case_argument/test_case_argument_multi_device_selection.h"
#include "framework/test_case_argument/test_case_argument_usm_device_selection.h"

#include <sstream>

struct UsmCopyArguments : TestCaseArguments {
    MultiDeviceSelectionTestCaseArgument contextPlacement;
    DeviceSelectionTestCaseArgument queuePlacement;
    UsmDeviceSelectionTestCaseArgument srcPlacement;
    UsmDeviceSelectionTestCaseArgument dstPlacement;
    ByteSizeTestCaseArgument size;
    BooleanTestCaseArgument copyQueue;
    BooleanTestCaseArgument useEvents;

    UsmCopyArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device will perform the fill"),
          srcPlacement(*this, "src", "Placement of memory for the buffer"),
          dstPlacement(*this, "dst", "Placement of memory for the buffer"),
          size(*this, "size", "Size of the buffer"),
          copyQueue(*this, "copyQueue", "Force blitter"),
          useEvents(*this, "useEvents", "Select if GPU-side measurements should be done") {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, queuePlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(srcPlacement)) &&
               DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(dstPlacement));
    }
};

class UsmCopy : public TestCase<UsmCopyArguments> {
  public:
    using TestCase<UsmCopyArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a USM buffer and measures fill bandwidth.";
    }

    std::string getTestCaseName() const override {
        return "UsmCopy";
    }
};
