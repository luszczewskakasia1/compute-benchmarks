#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct WriteBufferRectArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    BooleanTestCaseArgument compressed;
    ThreeComponentOffsetTestCaseArgument origin;
    ThreeComponentSizeTestCaseArgument region;
    ByteSizeTestCaseArgument rPitch;
    ByteSizeTestCaseArgument sPitch;

    WriteBufferRectArguments()
        : size(*this, "size"),
          compressed(*this, "compressed"),
          origin(*this, "origin"),
          region(*this, "region"),
          rPitch(*this, "rPitch"),
          sPitch(*this, "sPitch") {}
};

class WriteBufferRect : public TestCase<WriteBufferRectArguments> {
  public:
    using TestCase<WriteBufferRectArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures rectangle write bandwidth.";
    }

    std::string getTestCaseName() const override {
        return "WriteBufferRect";
    }
};
