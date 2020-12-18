#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct NewResourcesWithGpuAccessArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;

    NewResourcesWithGpuAccessArguments()
        : size(*this, "size", "Size of the buffer") {}
};

struct NewResourcesWithGpuAccess : TestCase<NewResourcesWithGpuAccessArguments> {
    using TestCase<NewResourcesWithGpuAccessArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "NewResourcesWithGpuAccess";
    }

    std::string getHelp() const override {
        return "enqueues kernel that accesses an entire buffer placed in device memory to measure "
               "resource preparation time. The resource is destroyed and recreated for each "
               "iteration to ensure it is a different memory allocation.";
    };
};
