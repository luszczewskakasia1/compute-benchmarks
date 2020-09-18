#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

#include <sstream>

struct CopyBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    CompressionBooleanTestCaseArgument compressedSource;
    CompressionBooleanTestCaseArgument compressedDestination;

    BooleanTestCaseArgument useEvents;

    CopyBufferArguments()
        : size(*this, "size"),
          compressedSource(*this, "compressedSource"),
          compressedDestination(*this, "compressedDestination"),
          useEvents(*this, "useEvents", "measure performance with events") {}
};

class CopyBuffer : public TestCase<CopyBufferArguments> {
  public:
    using TestCase<CopyBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates two OpenCL buffers and measures copy bandwidth between them.";
    }

    std::string getTestCaseName() const override {
        return "CopyBuffer";
    }
};
