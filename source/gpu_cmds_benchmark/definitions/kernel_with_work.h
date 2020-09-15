#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_work_item_id_usage.h"

struct KernelWithWorkArguments : TestCaseArguments {
    WorkItemIdUsageTestCaseArgument usedIds;
    PositiveIntegerTestCaseArgument measuredCommands;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    KernelWithWorkArguments()
        : usedIds(*this, "usedIds"),
          measuredCommands(*this, "measuredCommands", "Number of commands being measured. Result is divided by this number."),
          workgroupCount(*this, "wgc", "workgroup count"),
          workgroupSize(*this, "wgs", "workgroup size (aka local work size)") {}
};

class KernelWithWork : public TestCase<KernelWithWorkArguments> {
  public:
    using TestCase<KernelWithWorkArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time required to run a GPU kernel which assigns values to elements of a buffer.";
    }

    std::string getTestCaseName() const override {
        return "KernelWithWork";
    }
};
