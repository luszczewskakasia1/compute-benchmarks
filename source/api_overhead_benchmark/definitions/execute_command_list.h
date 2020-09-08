#pragma once

#include "framework/test_case/test_case.h"

struct ExecuteCommandListArguments : TestCaseArguments {
    BooleanTestCaseArgument useFence;

    ExecuteCommandListArguments()
        : useFence(*this, "useFence") {}
};

class ExecuteCommandList : public TestCase<ExecuteCommandListArguments> {
  public:
    using TestCase<ExecuteCommandListArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time spent in zeCommandQueueExecuteCommandLists on CPU.";
    }

    std::string getTestCaseName() const override {
        return "ExecuteCommandList";
    }
};
