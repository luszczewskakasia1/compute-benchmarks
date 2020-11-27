#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

#include <sstream>

struct FillBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    ByteSizeTestCaseArgument patternSize;
    CompressionBooleanTestCaseArgument compressed;
    BooleanTestCaseArgument forceBlitter;

    FillBufferArguments()
        : size(*this, "size"),
          patternSize(*this, "patternSize"),
          compressed(*this, "compressed"),
          forceBlitter(*this, "forceBlitter", "Force blitter engine. Test will be skipped if device does not support blitter. Warning: in OpenCL blitter may still be used even if not forced") {}
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
