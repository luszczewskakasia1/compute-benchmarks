#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

#include <sstream>

struct WriteBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    CompressionBooleanTestCaseArgument compressed;

    WriteBufferArguments()
        : size(*this, "size"),
          compressed(*this, "compressed") {}
};

class WriteBuffer : public TestCase<WriteBufferArguments> {
  public:
    using TestCase<WriteBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures write bandwidth.";
    }

    std::string getTestCaseName() const override {
        return "WriteBuffer";
    }
};
