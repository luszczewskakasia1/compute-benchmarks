#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/utility/common_help_message.h"

struct ReadBufferMisalignedArguments : Arguments {
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
