#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct FillBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    ByteSizeTestCaseArgument patternSize;
    BooleanTestCaseArgument compressed;

    FillBufferArguments()
        : size(*this, "size"),
          patternSize(*this, "patternSize"),
          compressed(*this, "compressed") {}
};

class FillBuffer : public TestCase<FillBufferArguments> {
  public:
    using TestCase<FillBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures fill bandwidth.";
    }

    std::string getTestCaseName() const override {
        return "FillBuffer";
    }
};
