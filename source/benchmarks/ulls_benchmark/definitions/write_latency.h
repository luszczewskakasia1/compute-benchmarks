#pragma once

#include "framework/test_case/test_case.h"

struct WriteLatencyArguments : TestCaseArguments {};

struct WriteLatency : TestCase<WriteLatencyArguments> {
    using TestCase<WriteLatencyArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "WriteLatency";
    }

    std::string getHelp() const override {
        return "unblocks event on GPU, then waits for timestamp being written.";
    }
};
