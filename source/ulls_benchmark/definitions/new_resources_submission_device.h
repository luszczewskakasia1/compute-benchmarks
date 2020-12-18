#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

#include <sstream>

struct NewResourcesSubmissionDeviceArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;

    NewResourcesSubmissionDeviceArguments()
        : size(*this, "size", "Size of the buffer") {}
};

struct NewResourcesSubmissionDevice : TestCase<NewResourcesSubmissionDeviceArguments> {
    using TestCase<NewResourcesSubmissionDeviceArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "NewResourcesSubmissionDevice";
    }

    std::string getHelp() const override {
        return "enqueues kernel that uses a buffer placed in device memory to measure resource "
               "preparation time. The resource is destroyed and recreated for each iteration "
               "to ensure it is a different memory allocation.";
    };
};
