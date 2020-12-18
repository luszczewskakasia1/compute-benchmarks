#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_map_flags.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/utility/common_help_message.h"

struct MapBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    CompressionBooleanTestCaseArgument compressed;
    MapFlagsTestCaseArgument mapFlags;
    BooleanTestCaseArgument useEvents;

    MapBufferArguments()
        : size(*this, "size", "Size of the buffer"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          mapFlags(*this, "mapFlags", "OpenCL map flags passed during memory mapping"),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct MapBuffer : TestCase<MapBufferArguments> {
    using TestCase<MapBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "MapBuffer";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures map bandwidth. Mapping operation means "
               "memory transfer from GPU to CPU or a no-op, depending on map flags.";
    }
};
