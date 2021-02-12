#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_multi_device_selection.h"
#include "framework/test_case_argument/enum/test_case_argument_usm_device_selection.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

struct UsmSharedMigrateGpuArguments : Arguments {
    MultiDeviceSelectionArgument contextPlacement;
    UsmSharedDeviceSelectionArgument bufferPlacement;
    ByteSizeArgument bufferSize;

    UsmSharedMigrateGpuArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          bufferPlacement(*this, "memory", "Placement of memory for the buffer"),
          bufferSize(*this, "size", "Size of the buffer") {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(bufferPlacement));
    }
};

struct UsmSharedMigrateGpu : TestCase<UsmSharedMigrateGpuArguments> {
    using TestCase<UsmSharedMigrateGpuArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmSharedMigrateGpu";
    }

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to migrate it from CPU to GPU.";
    }
};
