#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct UsmSharedMigrateGpuArguments : TestCaseArguments {
    ByteSizeTestCaseArgument bufferSize;

    UsmSharedMigrateGpuArguments()
        : bufferSize(*this, "size", "size of the buffer to be set") {}
};

class UsmSharedMigrateGpu : public TestCase<UsmSharedMigrateGpuArguments> {
  public:
    using TestCase<UsmSharedMigrateGpuArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to migrate it from CPU to GPU";
    }

    std::string getTestCaseName() const override {
        return "UsmSharedMigrateGpu";
    }
};
