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
        : workgroupCount(*this, "wgc", "workgroup count"),
          workgroupSize(*this, "wgs", "workgroup size"),
          useOoq(*this, "ooq", "use out of order queue"),
          useProfiling(*this, "profiling", "creating a profiling queue"),
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
