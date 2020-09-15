#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

#include <sstream>

struct UsmSharedMigrateCpuArguments : TestCaseArguments {
    BooleanTestCaseArgument accessAllBytes;
    ByteSizeTestCaseArgument bufferSize;

    UsmSharedMigrateCpuArguments()
        : accessAllBytes(*this, "accessAllBytes", "select, whether entire resource or only one byte will be accessed on CPU"),
          bufferSize(*this, "size", "size of the buffer to be set") {}
};

class UsmSharedMigrateCpu : public TestCase<UsmSharedMigrateCpuArguments> {
  public:
    using TestCase<UsmSharedMigrateCpuArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to migrate it from GPU to CPU";
    }

    std::string getTestCaseName() const override {
        return "UsmSharedMigrateCpu";
    }
};
