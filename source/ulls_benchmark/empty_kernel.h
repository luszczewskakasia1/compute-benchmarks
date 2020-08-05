#pragma once

#include "framework/test_case/test_case.h"

#include <sstream>

struct EmptyKernelArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    EmptyKernelArguments()
        : workgroupCount("wgc", "workgroup count"),
          workgroupSize("wgs", "workgroup size (aka local work size)") {
        arguments.push_back(&workgroupCount);
        arguments.push_back(&workgroupSize);
    }
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
