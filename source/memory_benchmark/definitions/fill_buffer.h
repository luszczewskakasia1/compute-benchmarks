#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

#include <sstream>

struct FillBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    ByteSizeTestCaseArgument patternSize;
    CompressionBooleanTestCaseArgument compressed;
    BooleanTestCaseArgument copyQueue;

    FillBufferArguments()
        : size(*this, "size"),
          patternSize(*this, "patternSize"),
          compressed(*this, "compressed"),
          copyQueue(*this, "copyQueue") {}
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
