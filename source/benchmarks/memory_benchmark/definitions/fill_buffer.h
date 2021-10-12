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

struct FillBufferArguments : TestCaseArgumentContainer {
    ByteSizeArgument size;
    BufferContentsArgument contents;
    ByteSizeArgument patternSize;
    CompressionBooleanArgument compressed;
    BooleanArgument forceBlitter;
    BooleanArgument useEvents;

    FillBufferArguments()
        : size(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffer"),
          patternSize(*this, "patternSize", "Size of the fill pattern"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          forceBlitter(*this, "forceBlitter", CommonHelpMessage::forceBlitter()),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct FillBuffer : TestCase<FillBufferArguments> {
    using TestCase<FillBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "FillBuffer";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures fill bandwidth. Buffer will be placed in "
               "device memory, if it's available.";
    }
};
