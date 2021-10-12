/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#pragma once

#include "framework/argument/compression_argument.h"
#include "framework/argument/enum/multi_device_selection_argument.h"
#include "framework/argument/enum/usm_device_selection_argument.h"
#include "framework/test_case/test_case.h"

struct UsmSharedMigrateGpuArguments : TestCaseArgumentContainer {
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
