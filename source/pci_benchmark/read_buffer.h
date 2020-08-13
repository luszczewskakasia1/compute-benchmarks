#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct ReadBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    BooleanTestCaseArgument compressed;

    ReadBufferArguments()
        : size(*this, "size"),
          compressed(*this, "compressed") {}
};

class ReadBuffer : public TestCase<ReadBufferArguments> {
  public:
    using TestCase<ReadBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates two OpenCL buffers and measures copy bandwidth between them.";
    }

    std::string getTestCaseName() const override {
        return "ReadBuffer";
    }
};
