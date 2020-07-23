#pragma once

#include "framework/test_case.h"

namespace UllsTest {

struct CompletionLatencyArguments : TestCaseArguments {};

class CompletionLatency : public TestCase<CompletionLatencyArguments> {
  public:
    using TestCase<CompletionLatencyArguments>::TestCase;

    std::string getHelp() override {
        return "enqueues system memory write and measures time between the moment when update is visible on CPU and"
               "the moment when synchronizing call returns\n";
    }

    std::string getTestCaseName() override {
        return "CompletionLatency";
    }
};

} // namespace UllsTest
