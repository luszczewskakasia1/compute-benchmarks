#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

#include <sstream>

struct EmptyKernelArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    EmptyKernelArguments()
        : workgroupCount(*this, "wgc", "Workgroup count"),
          workgroupSize(*this, "wgs", "Workgroup size (aka local work size)") {}
};

struct EmptyKernel : TestCase<EmptyKernelArguments> {
    using TestCase<EmptyKernelArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "EmptyKernel";
    }

    std::string getHelp() const override {
        return "enqueues empty kernel and measures time to launch it and wait for it on CPU, thus "
               "measuring walker spawn time.";
    }
};
