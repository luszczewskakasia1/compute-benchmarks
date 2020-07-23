#pragma once

#include "framework/test_case.h"

namespace UllsTest {

struct BestWalkerSubmissionArguments : TestCaseArguments {};

class BestWalkerSubmission : public TestCase<BestWalkerSubmissionArguments> {
  public:
    using TestCase<BestWalkerSubmissionArguments>::TestCase;

    std::string getHelp() override {
        return "enqueues kernel which updates system memory location and waits for the update in busy loop\n";
    }

    std::string getTestCaseName() override {
        return "BestWalkerSubmission";
    }
};

} // namespace UllsTest
