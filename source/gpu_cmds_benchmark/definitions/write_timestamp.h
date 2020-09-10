#pragma once

#include "framework/test_case/test_case.h"

struct WriteTimestampArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument measuredCommands;

    WriteTimestampArguments()
        : measuredCommands(*this, "measuredCommands", "Number of commands being measured. Result is divided by this number.") {}
};

class WriteTimestamp : public TestCase<WriteTimestampArguments> {
  public:
    using TestCase<WriteTimestampArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time required to write timestamp on GPU.";
    }

    std::string getTestCaseName() const override {
        return "WriteTimestamp";
    }
};
