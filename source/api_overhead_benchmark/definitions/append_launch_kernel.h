#pragma once

#include "framework/test_case/test_case.h"

struct AppendLaunchKernelArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument workgroupCount;
    IntegerTestCaseArgument workgroupSize;
    BooleanTestCaseArgument useEvent;

    AppendLaunchKernelArguments()
        : workgroupCount(*this, "wgc", "workgroup count"),
          workgroupSize(*this, "wgs", "workgroup size, pass 0 to make the driver calculate it during enqueue"),
          useEvent(*this, "event", "pass output event to the enqueue call") {}
};

class AppendLaunchKernel : public TestCase<AppendLaunchKernelArguments> {
  public:
    using TestCase<AppendLaunchKernelArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time spent in zeCommandListAppendLaunchKernel on CPU.";
    }

    std::string getTestCaseName() const override {
        return "AppendLaunchKernel";
    }
};
