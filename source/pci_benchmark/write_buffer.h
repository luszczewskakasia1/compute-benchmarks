#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct WriteBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    BooleanTestCaseArgument compressed;

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
