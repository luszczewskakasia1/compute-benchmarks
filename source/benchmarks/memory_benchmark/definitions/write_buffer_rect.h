#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/test_case_argument/test_case_argument_three_component_uint.h"
#include "framework/utility/common_help_message.h"

struct WriteBufferRectArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    CompressionBooleanTestCaseArgument compressed;
    ThreeComponentOffsetTestCaseArgument origin;
    ThreeComponentSizeTestCaseArgument region;
    ByteSizeTestCaseArgument rPitch;
    ByteSizeTestCaseArgument sPitch;

    WriteBufferRectArguments()
        : size(*this, "size", "Size of the buffer"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          origin(*this, "origin", "Origin of the rectangle"),
          region(*this, "region", "Size of the rectangle"),
          rPitch(*this, "rPitch", "Row pitch of the rectangle"),
          sPitch(*this, "sPitch", "Silice pitch of the rectangle") {}
};

struct WriteBufferRect : TestCase<WriteBufferRectArguments> {
    using TestCase<WriteBufferRectArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "WriteBufferRect";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures rectangle write bandwidth. Rectangle "
               "write operation means transfer from CPU to GPU.";
    }
};
