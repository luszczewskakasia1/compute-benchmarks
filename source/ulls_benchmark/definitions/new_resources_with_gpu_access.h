#pragma once

#include "framework/test_case/test_case.h"

#include <sstream>

struct NewResourcesWithGpuAccessArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;

    NewResourcesWithGpuAccessArguments()
        : size(*this, "size") {}
};

class NewResourcesWithGpuAccess : public TestCase<NewResourcesWithGpuAccessArguments> {
  public:
    using TestCase<NewResourcesWithGpuAccessArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel accessing the entire resource, which is new, and measures submission time.";
    };

    std::string getTestCaseName() const override {
        return "NewResourcesWithGpuAccess";
    }
};
