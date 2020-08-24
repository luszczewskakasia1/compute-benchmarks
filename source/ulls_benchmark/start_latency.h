#pragma once

#include "framework/test_case/test_case.h"

struct StartLatencyArguments : TestCaseArguments {};

class StartLatency : public TestCase<StartLatencyArguments> {
  public:
    using TestCase<StartLatencyArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues a kernel with profiling and checks delta between queue time and start time\n";
    }

    std::string getTestCaseName() const override {
        return "StartLatency";
    }
};
