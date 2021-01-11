#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/enum/test_case_argument_map_flags.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/utility/common_help_message.h"

struct UnmapBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    BufferContentsTestCaseArgument contents;
    CompressionBooleanTestCaseArgument compressed;
    MapFlagsTestCaseArgument mapFlags;
    BooleanTestCaseArgument useEvents;

    UnmapBufferArguments()
        : size(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffer"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          mapFlags(*this, "mapFlags", "OpenCL map flags passed during memory mapping"),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct UnmapBuffer : TestCase<UnmapBufferArguments> {
    using TestCase<UnmapBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UnmapBuffer";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures unmap bandwidth. Unmapping operation means"
               "memory transfer from CPU to GPU or a no-op, depending on map flags.";
    }
};
