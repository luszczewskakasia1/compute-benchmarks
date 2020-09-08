#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case/test_case_argument.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct MapBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    CompressionBooleanTestCaseArgument compressed;
    MapFlagsTestCaseArgument mapFlags;

    MapBufferArguments()
        : size(*this, "size"),
          compressed(*this, "compressed"),
          mapFlags(*this, "mapFlags") {}
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
