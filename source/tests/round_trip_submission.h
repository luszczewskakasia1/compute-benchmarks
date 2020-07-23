#pragma once

#include "framework/test_case.h"

namespace UllsTest {

struct RoundTripSubmissionArguments : TestCaseArguments {};

class RoundTripSubmission : public TestCase<RoundTripSubmissionArguments> {
  public:
    using TestCase<RoundTripSubmissionArguments>::TestCase;

    std::string getHelp() override {
        return "enqueues kernel which updates system memory location and waits for it with a synchronizing API\n";
    }

    std::string getTestCaseName() override {
        return "RoundTripSubmission";
    }
};

} // namespace UllsTest
