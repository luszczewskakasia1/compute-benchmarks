#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/test_case_argument/test_case_argument_device_selection.h"
#include "framework/test_case_argument/test_case_argument_multi_device_selection.h"
#include "framework/test_case_argument/test_case_argument_usm_device_selection.h"

#include <sstream>

struct UsmFillArguments : TestCaseArguments {
    MultiDeviceSelectionTestCaseArgument contextPlacement;
    DeviceSelectionTestCaseArgument queuePlacement;
    UsmDeviceSelectionTestCaseArgument bufferPlacement;
    ByteSizeTestCaseArgument size;
    ByteSizeTestCaseArgument patternSize;
    BooleanTestCaseArgument forceBlitter;
    BooleanTestCaseArgument useEvents;

    UsmFillArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device within the context will perform the operation"),
          bufferPlacement(*this, "memory", "Placement of memory for the buffer"),
          size(*this, "size", "Size of the buffer"),
          patternSize(*this, "patternSize", "Size of the fill pattern"),
          forceBlitter(*this, "copyQueue", "Force blitter"),
          useEvents(*this, "useEvents", "Select if GPU-side measurements should be done") {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, queuePlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(bufferPlacement));
    }
};

class UsmFill : public TestCase<UsmFillArguments> {
  public:
    using TestCase<UsmFillArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures fill bandwidth.";
    }

    std::string getTestCaseName() const override {
        return "UsmFill";
    }
};
