#pragma once

#include "framework/test_case/test_case.h"

struct WaitOnEventHotArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument measuredCommands;

    WaitOnEventHotArguments()
        : measuredCommands(*this, "measuredCommands", "Number of commands being measured. Result is divided by this number.") {}
};

class WaitOnEventHot : public TestCase<WaitOnEventHotArguments> {
  public:
    using TestCase<WaitOnEventHotArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time required to service a signalled semaphore, that was previously used";
    }

    std::string getTestCaseName() const override {
        return "WaitOnEventHot";
    }
};
