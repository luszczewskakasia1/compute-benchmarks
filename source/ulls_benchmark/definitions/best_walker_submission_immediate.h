#pragma once

#include "framework/test_case/test_case.h"

struct BestWalkerSubmissionImmediateArguments : TestCaseArguments {};

class BestWalkerSubmissionImmediate : public TestCase<BestWalkerSubmissionImmediateArguments> {
  public:
    using TestCase<BestWalkerSubmissionImmediateArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel which updates system memory location and waits for the update in busy loop. Kernel"
               "is enqueued using low-latency immediate command list, so the test is LevelZero only\n";
    }

    std::string getTestCaseName() const override {
        return "BestWalkerSubmissionImmediate";
    }
};
