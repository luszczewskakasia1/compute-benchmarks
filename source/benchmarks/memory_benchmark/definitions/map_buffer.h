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
#include "framework/argument/enum/map_flags_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct MapBufferArguments : TestCaseArgumentContainer {
    ByteSizeArgument size;
    BufferContentsArgument contents;
    CompressionBooleanArgument compressed;
    MapFlagsArgument mapFlags;
    BooleanArgument useEvents;

    MapBufferArguments()
        : size(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffer"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          mapFlags(*this, "mapFlags", "OpenCL map flags passed during memory mapping"),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct MapBuffer : TestCase<MapBufferArguments> {
    using TestCase<MapBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "MapBuffer";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures map bandwidth. Mapping operation means "
               "memory transfer from GPU to CPU or a no-op, depending on map flags.";
    }
};
