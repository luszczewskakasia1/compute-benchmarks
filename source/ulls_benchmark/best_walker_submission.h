#pragma once

#include "framework/test_case.h"

struct BestWalkerSubmissionArguments : TestCaseArguments {};

class BestWalkerSubmission : public TestCase<BestWalkerSubmissionArguments> {
  public:
    using TestCase<BestWalkerSubmissionArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel which updates system memory location and waits for the update in busy loop\n";
    }

    std::string getTestCaseName() const override {
        return "BestWalkerSubmission";
    }
};
