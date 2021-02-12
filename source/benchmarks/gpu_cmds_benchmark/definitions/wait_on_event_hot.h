#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct WaitOnEventHotArguments : Arguments {
    PositiveIntegerArgument measuredCommands;

    WaitOnEventHotArguments()
        : measuredCommands(*this, "measuredCommands", CommonHelpMessage::measuredCommandsCount()) {}
};

struct WaitOnEventHot : TestCase<WaitOnEventHotArguments> {
    using TestCase<WaitOnEventHotArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "WaitOnEventHot";
    }

    std::string getHelp() const override {
        return "measures time required to service a signalled semaphore, that was previously used";
    }
};
