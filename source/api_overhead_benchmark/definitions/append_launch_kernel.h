#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct AppendLaunchKernelArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument workgroupCount;
    IntegerTestCaseArgument workgroupSize;
    BooleanTestCaseArgument useEvent;

    AppendLaunchKernelArguments()
        : workgroupCount(*this, "wgc", "Workgroup count"),
          workgroupSize(*this, "wgs", "Workgroup size, pass 0 to make the driver calculate it during enqueue"),
          useEvent(*this, "event", "Pass output event to the enqueue call") {}
};

struct AppendLaunchKernel : TestCase<AppendLaunchKernelArguments> {
    using TestCase<AppendLaunchKernelArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "AppendLaunchKernel";
    }

    std::string getHelp() const override {
        return "measures time spent in zeCommandListAppendLaunchKernel on CPU.";
    }
};
