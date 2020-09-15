#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

#include <sstream>

struct UsmSharedFirstCpuAccessArguments : TestCaseArguments {
    ByteSizeTestCaseArgument bufferSize;

    UsmSharedFirstCpuAccessArguments()
        : bufferSize(*this, "size", "size of the buffer to be set") {}
};

class UsmSharedFirstCpuAccess : public TestCase<UsmSharedFirstCpuAccessArguments> {
  public:
    using TestCase<UsmSharedFirstCpuAccessArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to access it on cpu after creation";
    }

    std::string getTestCaseName() const override {
        return "UsmSharedFirstCpuAccess";
    }
};
