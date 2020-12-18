#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct EnqueueNdrNullLwsArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument gws;
    BooleanTestCaseArgument useOoq;
    BooleanTestCaseArgument useProfiling;
    BooleanTestCaseArgument useEvent;

    EnqueueNdrNullLwsArguments()
        : gws(*this, "gws", "Blobal work size"),
          useOoq(*this, "ooq", "Use out of order queue"),
          useProfiling(*this, "profiling", "Creating a profiling queue"),
          useEvent(*this, "event", "Pass output event to the enqueue call") {}
};

struct EnqueueNdrNullLws : TestCase<EnqueueNdrNullLwsArguments> {
    using TestCase<EnqueueNdrNullLwsArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "EnqueueNdrNullLws";
    }

    std::string getHelp() const override {
        return "measures time spent in clEnqueueNDRangeKernel on CPU. Null LWS is provided, which "
               "causes driver to calculate it";
    }
};
