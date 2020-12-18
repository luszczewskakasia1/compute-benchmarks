#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct UsmSharedMigrateCpuArguments : TestCaseArguments {
    BooleanTestCaseArgument accessAllBytes;
    ByteSizeTestCaseArgument bufferSize;

    UsmSharedMigrateCpuArguments()
        : accessAllBytes(*this, "accessAllBytes", "Select, whether entire resource or only one byte will be accessed on CPU"),
          bufferSize(*this, "size", "Size of the buffer") {}
};

struct UsmSharedMigrateCpu : TestCase<UsmSharedMigrateCpuArguments> {
    using TestCase<UsmSharedMigrateCpuArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmSharedMigrateCpu";
    }

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to migrate it from GPU to CPU";
    }
};
