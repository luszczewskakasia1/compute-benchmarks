#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/utility/common_help_message.h"

struct FillBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    BufferContentsTestCaseArgument contents;
    ByteSizeTestCaseArgument patternSize;
    CompressionBooleanTestCaseArgument compressed;
    BooleanTestCaseArgument forceBlitter;
    BooleanTestCaseArgument useEvents;

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
