#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_multi_device_selection.h"
#include "framework/test_case_argument/enum/test_case_argument_usm_device_selection.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

struct UsmSharedMigrateCpuArguments : Arguments {
    MultiDeviceSelectionArgument contextPlacement;
    UsmSharedDeviceSelectionArgument bufferPlacement;
    ByteSizeArgument bufferSize;
    BooleanArgument accessAllBytes;

    UsmSharedMigrateCpuArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          bufferPlacement(*this, "memory", "Placement of memory for the buffer"),
          bufferSize(*this, "size", "Size of the buffer"),
          accessAllBytes(*this, "accessAllBytes", "Select, whether entire resource or only one byte will be accessed on CPU") {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(bufferPlacement));
    }
};

struct UsmSharedMigrateCpu : TestCase<UsmSharedMigrateCpuArguments> {
    using TestCase<UsmSharedMigrateCpuArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmSharedMigrateCpu";
    }

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to migrate it from GPU to CPU.";
    }
};
