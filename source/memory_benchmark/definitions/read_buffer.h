#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/utility/common_help_message.h"

struct ReadBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    BufferContentsTestCaseArgument contents;
    CompressionBooleanTestCaseArgument compressed;
    BooleanTestCaseArgument useEvents;

    ReadBufferArguments()
        : size(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffer"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct ReadBuffer : TestCase<ReadBufferArguments> {
    using TestCase<ReadBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "ReadBuffer";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures read bandwidth. Read operation means "
               "transfer from GPU to CPU.";
    }
};
