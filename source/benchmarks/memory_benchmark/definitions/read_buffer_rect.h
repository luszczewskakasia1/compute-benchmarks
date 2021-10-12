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
#include "framework/argument/three_component_uint_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct ReadBufferRectArguments : TestCaseArgumentContainer {
    ByteSizeArgument size;
    CompressionBooleanArgument compressed;
    ThreeComponentOffsetArgument origin;
    ThreeComponentSizeArgument region;
    ByteSizeArgument rPitch;
    ByteSizeArgument sPitch;

    ReadBufferRectArguments()
        : size(*this, "size", "Size of the buffer"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          origin(*this, "origin", "Origin of the rectangle"),
          region(*this, "region", "Size of the rectangle"),
          rPitch(*this, "rPitch", "Row pitch of the rectangle"),
          sPitch(*this, "sPitch", "Silice pitch of the rectangle") {}
};

struct ReadBufferRect : TestCase<ReadBufferRectArguments> {
    using TestCase<ReadBufferRectArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "ReadBufferRect";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures rectangle read bandwidth. Rectangle "
               "read operation means transfer from GPU to CPU.";
    }
};
