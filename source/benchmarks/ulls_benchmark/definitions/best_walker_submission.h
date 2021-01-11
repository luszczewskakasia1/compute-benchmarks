#pragma once

#include "framework/test_case/test_case.h"

struct BestWalkerSubmissionArguments : TestCaseArguments {};

struct BestWalkerSubmission : TestCase<BestWalkerSubmissionArguments> {
    using TestCase<BestWalkerSubmissionArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "BestWalkerSubmission";
    }

    std::string getHelp() const override {
        return "enqueues kernel, which updates system memory location and then busy-loops on CPU "
               "until the update becomes visible.";
    }
};
