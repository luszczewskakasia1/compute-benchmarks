#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct EnqueueNdrTimeArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;
    BooleanTestCaseArgument useOoq;
    BooleanTestCaseArgument useProfiling;
    BooleanTestCaseArgument useEvent;

    EnqueueNdrTimeArguments()
        : workgroupCount(*this, "wgc", "Workgroup count"),
          workgroupSize(*this, "wgs", "Workgroup size"),
          useOoq(*this, "ooq", "Use out of order queue"),
          useProfiling(*this, "profiling", "Creating a profiling queue"),
          useEvent(*this, "event", "Pass output event to the enqueue call") {}
};

struct EnqueueNdrTime : TestCase<EnqueueNdrTimeArguments> {
    using TestCase<EnqueueNdrTimeArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "EnqueueNdrTime";
    }

    std::string getHelp() const override {
        return "measures time spent in clEnqueueNDRangeKernel on CPU.";
    }
};
