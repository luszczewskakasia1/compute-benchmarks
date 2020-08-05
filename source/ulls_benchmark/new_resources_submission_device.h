#pragma once

#include "framework/test_case/test_case.h"

#include <sstream>

struct NewResourcesSubmissionDeviceArguments : TestCaseArguments {
    PositiveIntegerTestCaseArgument size;

    NewResourcesSubmissionDeviceArguments()
        : size("size") {
        arguments.push_back(&size);
    }
};

class NewResourcesSubmissionDevice : public TestCase<NewResourcesSubmissionDeviceArguments> {
  public:
    using TestCase<NewResourcesSubmissionDeviceArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel with a new device resource to measure resource preparation time.";
    };

    std::string getTestCaseName() const override {
        return "NewResourcesSubmissionDevice";
    }
};
