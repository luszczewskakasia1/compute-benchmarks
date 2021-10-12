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

struct CopyBufferArguments : TestCaseArgumentContainer {
    MultiDeviceSelectionArgument contextPlacement;
    DeviceSelectionArgument queuePlacement;
    DeviceSelectionArgument srcPlacement;
    DeviceSelectionArgument dstPlacement;
    ByteSizeArgument size;
    CompressionBooleanArgument srcCompressed;
    CompressionBooleanArgument dstCompressed;
    BooleanArgument useEvents;

    CopyBufferArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device within the context will perform the operation"),
          srcPlacement(*this, "src", "Placement of memory for the source buffer"),
          dstPlacement(*this, "dst", "Placement of memory for the destination buffer"),
          size(*this, "size", "Size of the buffers"),
          srcCompressed(*this, "srcCompressed", CommonHelpMessage::compression("source buffer")),
          dstCompressed(*this, "dstCompressed", CommonHelpMessage::compression("destination buffer")),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, queuePlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, srcPlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, dstPlacement);
    }
};

struct CopyBuffer : TestCase<CopyBufferArguments> {
    using TestCase<CopyBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "CopyBuffer";
    }

    std::string getHelp() const override {
        return "allocates two OpenCL buffers and measures copy bandwidth between them. Buffers "
               "will be placed in device memory, if it's available.";
    }
};
