#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

#include <sstream>

struct NewResourcesSubmissionHostArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;

    NewResourcesSubmissionHostArguments()
        : size(*this, "size") {}
};

class NewResourcesSubmissionHost : public TestCase<NewResourcesSubmissionHostArguments> {
  public:
    using TestCase<NewResourcesSubmissionHostArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel with a new host resource to measure resource preparation time.";
    };

    std::string getTestCaseName() const override {
        return "NewResourcesSubmissionHost";
    }
};
