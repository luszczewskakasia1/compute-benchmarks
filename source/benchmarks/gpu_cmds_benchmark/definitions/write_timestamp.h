#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct WriteTimestampArguments : Arguments {
    PositiveIntegerArgument measuredCommands;

    WriteTimestampArguments()
        : measuredCommands(*this, "measuredCommands", CommonHelpMessage::measuredCommandsCount()) {}
};

struct WriteTimestamp : TestCase<WriteTimestampArguments> {
    using TestCase<WriteTimestampArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "WriteTimestamp";
    }

    std::string getHelp() const override {
        return "measures time required to write a timestamp on GPU.";
    }
};
