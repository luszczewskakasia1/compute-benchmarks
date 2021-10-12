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
#include "framework/argument/enum/device_selection_argument.h"
#include "framework/argument/enum/multi_device_selection_argument.h"
#include "framework/argument/enum/usm_device_selection_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct UsmCopyArguments : TestCaseArgumentContainer {
    MultiDeviceSelectionArgument contextPlacement;
    DeviceSelectionArgument queuePlacement;
    UsmDeviceSelectionArgument srcPlacement;
    UsmDeviceSelectionArgument dstPlacement;
    ByteSizeArgument size;
    BooleanArgument forceBlitter;
    BooleanArgument useEvents;

    UsmCopyArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device within the context will perform the operation"),
          srcPlacement(*this, "src", "Placement of memory for the source buffer"),
          dstPlacement(*this, "dst", "Placement of memory for the destination buffer"),
          size(*this, "size", "Size of the buffers"),
          forceBlitter(*this, "forceBlitter", CommonHelpMessage::forceBlitter()),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, queuePlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(srcPlacement)) &&
               DeviceSelectionHelper::isSubset(contextPlacement, DeviceSelectionHelper::withoutHost(dstPlacement));
    }
};

struct UsmCopy : TestCase<UsmCopyArguments> {
    using TestCase<UsmCopyArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmCopy";
    }

    std::string getHelp() const override {
        return "allocates two unified shared memory buffers and measures copy bandwidth between "
               "them using a builtin function.";
    }
};
