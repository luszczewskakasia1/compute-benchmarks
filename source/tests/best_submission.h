#pragma once

#include "framework/test_case.h"

namespace UllsTest {

struct BestSubmissionArguments : TestCaseArguments {};

class BestSubmission : public TestCase<BestSubmissionArguments> {
  public:
    using TestCase<BestSubmissionArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues a system memory write without kernel and measure when update when became visible on the CPUs\n";
    }

    std::string getTestCaseName() const override {
        return "BestSubmission";
    }
};

} // namespace UllsTest
