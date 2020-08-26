#pragma once

#include "framework/test_case/test_case.h"

struct RoundTripSubmissionArguments : TestCaseArguments {};

class RoundTripSubmission : public TestCase<RoundTripSubmissionArguments> {
  public:
    using TestCase<RoundTripSubmissionArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel which updates system memory location and waits for it with a synchronizing API\n";
    }

    std::string getTestCaseName() const override {
        return "RoundTripSubmission";
    }
};
