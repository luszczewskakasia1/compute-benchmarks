#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct UsmSharedFirstGpuAccessArguments : TestCaseArguments {
    ByteSizeTestCaseArgument bufferSize;

    UsmSharedFirstGpuAccessArguments()
        : bufferSize(*this, "size", "size of the buffer to be set") {}
};

class UsmSharedFirstGpuAccess : public TestCase<UsmSharedFirstGpuAccessArguments> {
  public:
    using TestCase<UsmSharedFirstGpuAccessArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to access it on gpu after creation";
    }

    std::string getTestCaseName() const override {
        return "UsmSharedFirstGpuAccess";
    }
};
