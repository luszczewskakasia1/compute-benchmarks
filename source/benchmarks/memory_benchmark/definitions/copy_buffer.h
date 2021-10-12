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
#include "framework/argument/enum/device_selection_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct CopyBufferArguments : TestCaseArgumentContainer {
    ByteSizeArgument size;
    BufferContentsArgument contents;
    CompressionBooleanArgument compressedSource;
    CompressionBooleanArgument compressedDestination;
    BooleanArgument useEvents;

    CopyBufferArguments()
        : size(*this, "size", "Size of the buffers"),
          contents(*this, "contents", "Contents of the buffers"),
          compressedSource(*this, "compressedSource", CommonHelpMessage::compression("source buffer")),
          compressedDestination(*this, "compressedDestination", CommonHelpMessage::compression("destination buffer")),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
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
