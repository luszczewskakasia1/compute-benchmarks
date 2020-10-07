#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct EnqueueNdrNullLwsArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument gws;
    BooleanTestCaseArgument useOoq;
    BooleanTestCaseArgument useProfiling;
    BooleanTestCaseArgument useEvent;

    EnqueueNdrNullLwsArguments()
        : gws(*this, "gws", "global work size"),
          useOoq(*this, "ooq", "use out of order queue"),
          useProfiling(*this, "profiling", "creating a profiling queue"),
          useEvent(*this, "event", "pass output event to the enqueue call") {}
};

class EnqueueNdrNullLws : public TestCase<EnqueueNdrNullLwsArguments> {
  public:
    using TestCase<EnqueueNdrNullLwsArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time spent in clEnqueueNDRangeKernel on CPU. Null LWS is provided, which causes driver to calculate it";
    }

    std::string getTestCaseName() const override {
        return "EnqueueNdrNullLws";
    }
};
