#pragma once

#include "framework/test_case/test_case.h"

struct WriteLatencyArguments : TestCaseArguments {};

class WriteLatency : public TestCase<WriteLatencyArguments> {
public:
    using TestCase<WriteLatencyArguments>::TestCase;

    std::string getHelp() const override {
        return "unblocks event on GPU, then waits for timestamp being written\n";
    }

    std::string getTestCaseName() const override {
        return "WriteLatency";
    }
};
