#pragma once

#include "framework/test_case/test_case.h"

struct EnqueueNdrTimeArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument workgroupCount;
    IntegerTestCaseArgument workgroupSize;
    BooleanTestCaseArgument useEvent;

    EnqueueNdrTimeArguments()
        : workgroupCount(*this, "wgc", "workgroup count"),
          workgroupSize(*this, "wgs", "workgroup size, pass 0 to make the driver calculate it during enqueue"),
          useEvent(*this, "event", "pass output event to the enqueue call") {}
};

class EnqueueNdrTime : public TestCase<EnqueueNdrTimeArguments> {
  public:
    using TestCase<EnqueueNdrTimeArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time spent in clEnqueueNDRangeKernel on CPU.";
    }

    std::string getTestCaseName() const override {
        return "EnqueueNdrTime";
    }
};
