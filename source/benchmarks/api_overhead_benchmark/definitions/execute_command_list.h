#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct ExecuteCommandListArguments : Arguments {
    BooleanArgument useFence;

    ExecuteCommandListArguments()
        : useFence(*this, "UseFence", "Pass a non-null ze_fence_handle_t to the API call") {}
};

struct ExecuteCommandList : TestCase<ExecuteCommandListArguments> {
    using TestCase<ExecuteCommandListArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "ExecuteCommandList";
    }

    std::string getHelp() const override {
        return "measures time spent in zeCommandQueueExecuteCommandLists on CPU.";
    }
};
