#pragma once

#include "framework/test_case/test_case.h"

struct WalkerCompletionLatencyArguments : TestCaseArguments {};

class WalkerCompletionLatency : public TestCase<WalkerCompletionLatencyArguments> {
  public:
    using TestCase<WalkerCompletionLatencyArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues a kernel writing to system memory and measures time between the moment when "
               "update is visible on CPU and the moment when synchronizing call returns\n";
    }

    std::string getTestCaseName() const override {
        return "WalkerCompletionLatency";
    }
};
