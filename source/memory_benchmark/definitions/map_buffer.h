#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_map_flags.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

#include <sstream>

struct MapBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    CompressionBooleanTestCaseArgument compressed;
    MapFlagsTestCaseArgument mapFlags;
    BooleanTestCaseArgument useEvents;

    MapBufferArguments()
        : size(*this, "size"),
          compressed(*this, "compressed"),
          mapFlags(*this, "mapFlags"),
          useEvents(*this, "useEvents") {}
};

class MapBuffer : public TestCase<MapBufferArguments> {
  public:
    using TestCase<MapBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures unmap bandwidth";
    }

    std::string getTestCaseName() const override {
        return "MapBuffer";
    }
};
