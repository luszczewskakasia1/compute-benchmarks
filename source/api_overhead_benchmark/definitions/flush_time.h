#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct FlushTimeArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument workgroupCount;
    IntegerTestCaseArgument workgroupSize;
    BooleanTestCaseArgument useOoq;
    BooleanTestCaseArgument useEvent;

    FlushTimeArguments()
        : workgroupCount(*this, "wgc", "Workgroup count"),
          workgroupSize(*this, "wgs", "Workgroup size, pass 0 to make the driver calculate it during enqueue"),
          useOoq(*this, "ooq", "Use out of order queue"),
          useEvent(*this, "event", "Pass output event to the enqueue call") {}
};

struct FlushTime : TestCase<FlushTimeArguments> {
    using TestCase<FlushTimeArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "FlushTime";
    }

    std::string getHelp() const override {
        return "measures time spent in clEnqueueNDRangeKernel on CPU.";
    }
};
