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
#include "framework/argument/enum/buffer_contents_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct ReadBufferMisalignedArguments : TestCaseArgumentContainer {
    ByteSizeArgument size;
    NonNegativeIntegerArgument misalignmentFromCacheline;
    BooleanArgument useEvents;

    ReadBufferMisalignedArguments()
        : size(*this, "size", "Size of the buffer"),
          misalignmentFromCacheline(*this, "misalignment", "Number of bytes by which misaligned the destination pointer will be misaligned"),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct ReadBufferMisaligned : TestCase<ReadBufferMisalignedArguments> {
    using TestCase<ReadBufferMisalignedArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "ReadBufferMisaligned";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures read bandwidth. Read operation means "
               "transfer from GPU to CPU. Destination pointer passed by the application will be "
               "misaligned by the specified amount of bytes.";
    }
};
