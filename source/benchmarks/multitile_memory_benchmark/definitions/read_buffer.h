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
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct ReadBufferArguments : TestCaseArgumentContainer {
    MultiDeviceSelectionArgument contextPlacement;
    DeviceSelectionArgument queuePlacement;
    DeviceSelectionArgument bufferPlacement;
    ByteSizeArgument size;
    CompressionBooleanArgument compressed;
    BooleanArgument useEvents;

    ReadBufferArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device within the context will perform the operation"),
          bufferPlacement(*this, "memory", "Placement of memory for the buffer"),
          size(*this, "size", "Size of the buffer"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, queuePlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, bufferPlacement);
    }
};

struct ReadBuffer : TestCase<ReadBufferArguments> {
    using TestCase<ReadBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "ReadBuffer";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures read bandwidth. Read operation means "
               "transfer from GPU to CPU.";
    }
};
