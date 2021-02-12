#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct NewResourcesSubmissionHostArguments : Arguments {
    ByteSizeArgument size;

    NewResourcesSubmissionHostArguments()
        : size(*this, "size", "Size of the buffer") {}
};

struct NewResourcesSubmissionHost : TestCase<NewResourcesSubmissionHostArguments> {
    using TestCase<NewResourcesSubmissionHostArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "NewResourcesSubmissionHost";
    }

    std::string getHelp() const override {
        return "enqueues kernel that uses a buffer placed in host memory to measure resource "
               "preparation time. The resource is destroyed and recreated for each iteration "
               "to ensure it is a different memory allocation.";
    };
};
