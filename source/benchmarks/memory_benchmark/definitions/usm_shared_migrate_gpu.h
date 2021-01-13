#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct UsmSharedMigrateGpuArguments : TestCaseArguments {
    ByteSizeTestCaseArgument bufferSize;

    UsmSharedMigrateGpuArguments()
        : bufferSize(*this, "size", "Size of the buffer") {}
};

struct UsmSharedMigrateGpu : TestCase<UsmSharedMigrateGpuArguments> {
    using TestCase<UsmSharedMigrateGpuArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmSharedMigrateGpu";
    }

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to migrate it from CPU to GPU";
    }
};
