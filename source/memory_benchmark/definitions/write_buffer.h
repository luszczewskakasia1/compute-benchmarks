#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/utility/common_help_message.h"

struct WriteBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    BufferContentsTestCaseArgument contents;
    CompressionBooleanTestCaseArgument compressed;
    BooleanTestCaseArgument useEvents;

    WriteBufferArguments()
        : size(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffer"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct WriteBuffer : TestCase<WriteBufferArguments> {
    using TestCase<WriteBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "WriteBuffer";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures write bandwidth. Write operation means "
               "transfer from CPU to GPU.";
    }
};
