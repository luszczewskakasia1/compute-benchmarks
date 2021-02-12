#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/enum/test_case_argument_memory_placement.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct UsmFillArguments : Arguments {
    MemoryPlacementArgument memoryPlacement;
    ByteSizeArgument bufferSize;
    BufferContentsArgument contents;
    ByteSizeArgument patternSize;
    BufferContentsArgument patternContents;
    BooleanArgument forceBlitter;
    BooleanArgument useEvents;

    UsmFillArguments()
        : memoryPlacement(*this, "memory", "Placement of the buffer"),
          bufferSize(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffer"),
          patternSize(*this, "patternSize", "Size of the fill pattern"),
          patternContents(*this, "patternContents", "Select contents of the fill pattern"),
          forceBlitter(*this, "forceBlitter", CommonHelpMessage::forceBlitter()),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct UsmFill : TestCase<UsmFillArguments> {
    using TestCase<UsmFillArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmFill";
    }

    std::string getHelp() const override {
        return "allocates a unified memory buffer and measures fill bandwidth";
    }
};
