#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

#include <sstream>

struct EmptyKernelArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    EmptyKernelArguments()
        : workgroupCount(*this, "wgc", "workgroup count"),
          workgroupSize(*this, "wgs", "workgroup size (aka local work size)") {}
};

class EmptyKernel : public TestCase<EmptyKernelArguments> {
  public:
    using TestCase<EmptyKernelArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues empty kernel to measure walker spawn time.";
    }

    std::string getTestCaseName() const override {
        return "EmptyKernel";
    }
};
