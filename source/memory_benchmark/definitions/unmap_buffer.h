#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_map_flags.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

#include <sstream>

struct UnmapBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    CompressionBooleanTestCaseArgument compressed;
    MapFlagsTestCaseArgument mapFlags;

    UnmapBufferArguments()
        : size(*this, "size"),
          compressed(*this, "compressed"),
          mapFlags(*this, "mapFlags") {}
};

class UnmapBuffer : public TestCase<UnmapBufferArguments> {
  public:
    using TestCase<UnmapBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures map bandwidth.";
    }

    std::string getTestCaseName() const override {
        return "UnmapBuffer";
    }
};
