#pragma once

#include "framework/test_case/test_case.h"

struct WalkerSubmissionEventsArguments : TestCaseArguments {};

class WalkerSubmissionEvents : public TestCase<WalkerSubmissionEventsArguments> {
  public:
    using TestCase<WalkerSubmissionEventsArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues an empty kernel with profiling and checks delta between queue time and start time\n";
    }

    std::string getTestCaseName() const override {
        return "WalkerSubmissionEvents";
    }
};
