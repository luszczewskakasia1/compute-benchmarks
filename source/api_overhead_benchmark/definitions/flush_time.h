#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct FlushTimeArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument workgroupCount;
    IntegerTestCaseArgument workgroupSize;
    BooleanTestCaseArgument useOoq;
    BooleanTestCaseArgument useEvent;

    FlushTimeArguments()
        : workgroupCount(*this, "wgc", "workgroup count"),
          workgroupSize(*this, "wgs", "workgroup size, pass 0 to make the driver calculate it during enqueue"),
          useOoq(*this, "ooq", "use out of order queue"),
          useEvent(*this, "event", "pass output event to the enqueue call") {}
};

class FlushTime : public TestCase<FlushTimeArguments> {
  public:
    using TestCase<FlushTimeArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time spent in clEnqueueNDRangeKernel on CPU.";
    }

    std::string getTestCaseName() const override {
        return "FlushTime";
    }
};
