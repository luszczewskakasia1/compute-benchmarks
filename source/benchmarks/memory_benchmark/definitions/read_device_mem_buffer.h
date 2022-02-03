/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021-2022 Intel Corporation
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
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct ReadDeviceMemBufferArguments : TestCaseArgumentContainer {
    ByteSizeArgument size;
    CompressionBooleanArgument compressed;
    // ByteSizeArgument numWaves;

    ReadDeviceMemBufferArguments()
        : size(*this, "size", "Size of the buffer"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer"))
    //,numWaves(*this, "waves")
    {}
};

struct ReadDeviceMemBuffer : TestCase<ReadDeviceMemBufferArguments> {
    using TestCase<ReadDeviceMemBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "ReadDeviceMemBuffer";
    }

    std::string getHelp() const override {
        return "allocates two OpenCL buffers and measures source buffer read bandwidth. Source "
               "buffer resides in device memory.";
    }
};
