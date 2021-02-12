#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/enum/test_case_argument_memory_placement.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct UsmMemsetArguments : Arguments {
    MemoryPlacementArgument memoryPlacement;
    ByteSizeArgument bufferSize;
    BufferContentsArgument contents;
    BooleanArgument forceBlitter;
    BooleanArgument useEvents;

    UsmMemsetArguments()
        : memoryPlacement(*this, "memory", "Placement of the buffer"),
          bufferSize(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffer"),
          forceBlitter(*this, "forceBlitter", CommonHelpMessage::forceBlitter()),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct UsmMemset : TestCase<UsmMemsetArguments> {
    using TestCase<UsmMemsetArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmMemset";
    }

    std::string getHelp() const override {
        return "allocates a unified memory buffer and measures memset bandwidth";
    }
};
