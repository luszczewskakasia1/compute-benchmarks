#pragma once

#include "framework/test_case/test_case.h"

struct WaitOnEventColdArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument measuredCommands;

    WaitOnEventColdArguments()
        : measuredCommands(*this, "measuredCommands", "Number of commands being measured. Result is divided by this number.") {}
};

class WaitOnEventCold : public TestCase<WaitOnEventColdArguments> {
  public:
    using TestCase<WaitOnEventColdArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time required to service a signalled semaphore, that has never been waited for.";
    }

    std::string getTestCaseName() const override {
        return "WaitOnEventCold";
    }
};
