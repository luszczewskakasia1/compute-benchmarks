#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/test_case_argument/test_case_argument_multi_device_selection.h"
#include "framework/test_case_argument/test_case_argument_usm_device_selection.h"

#include <sstream>

struct UsmSharedMigrateCpuArguments : TestCaseArguments {
    MultiDeviceSelectionTestCaseArgument contextPlacement;
    UsmSharedDeviceSelectionTestCaseArgument bufferPlacement;
    ByteSizeTestCaseArgument bufferSize;
    BooleanTestCaseArgument accessAllBytes;

    UsmSharedMigrateCpuArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          bufferPlacement(*this, "memory", "Placement of memory for the buffer"),
          bufferSize(*this, "size", "Size of the buffer"),
          accessAllBytes(*this, "accessAllBytes", "Select if GPU-side measurements should be done") {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(bufferPlacement));
    }
};

class UsmSharedMigrateCpu : public TestCase<UsmSharedMigrateCpuArguments> {
  public:
    using TestCase<UsmSharedMigrateCpuArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to migrate it from GPU to CPU.";
    }

    std::string getTestCaseName() const override {
        return "UsmSharedMigrateCpu";
    }
};
