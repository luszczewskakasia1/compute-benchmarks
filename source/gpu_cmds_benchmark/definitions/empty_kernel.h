#pragma once

#include "framework/test_case/test_case.h"

struct EmptyKernelArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument measuredCommands;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    EmptyKernelArguments()
        : measuredCommands(*this, "measuredCommands", "Number of commands being measured. Result is divided by this number."),
          workgroupCount(*this, "wgc", "workgroup count"),
          workgroupSize(*this, "wgs", "workgroup size (aka local work size)") {}
};

class EmptyKernel : public TestCase<EmptyKernelArguments> {
  public:
    using TestCase<EmptyKernelArguments>::TestCase;

    std::string getHelp() const override {
        return "measures time required to run an empty kernel on GPU.";
    }

    std::string getTestCaseName() const override {
        return "EmptyKernel";
    }
};
