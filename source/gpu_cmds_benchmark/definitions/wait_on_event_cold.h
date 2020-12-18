#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct WaitOnEventColdArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument measuredCommands;

    WaitOnEventColdArguments()
        : measuredCommands(*this, "measuredCommands", CommonHelpMessage::measuredCommandsCount()) {}
};

struct WaitOnEventCold : TestCase<WaitOnEventColdArguments> {
    using TestCase<WaitOnEventColdArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "WaitOnEventCold";
    }

    std::string getHelp() const override {
        return "measures time required to service a signalled semaphore, that has never been waited for.";
    }
};
