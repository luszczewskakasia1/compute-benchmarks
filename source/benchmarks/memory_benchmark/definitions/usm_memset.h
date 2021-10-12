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

#include "framework/argument/basic_argument.h"
#include "framework/argument/enum/buffer_contents_argument.h"
#include "framework/argument/enum/usm_memory_placement_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct UsmMemsetArguments : TestCaseArgumentContainer {
    UsmMemoryPlacementArgument usmMemoryPlacement;
    ByteSizeArgument bufferSize;
    BufferContentsArgument contents;
    BooleanArgument forceBlitter;
    BooleanArgument useEvents;

    UsmMemsetArguments()
        : usmMemoryPlacement(*this, "memory", "Placement of the buffer"),
          bufferSize(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffer"),
          forceBlitter(*this, "forceBlitter", CommonHelpMessage::forceBlitter()),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct UsmMemset : TestCase<UsmMemsetArguments> {
    using TestCase<UsmMemsetArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmMemset";
    }

    std::string getHelp() const override {
        return "allocates a unified memory buffer and measures memset bandwidth";
    }
};
