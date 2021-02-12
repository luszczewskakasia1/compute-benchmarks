#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/enum/test_case_argument_memory_placement.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_long_hex.h"
#include "framework/utility/common_help_message.h"

struct UsmFillSpecificPatternArguments : Arguments {
    MemoryPlacementArgument memoryPlacement;
    ByteSizeArgument bufferSize;
    BufferContentsArgument contents;
    LongHexArgument pattern;
    BooleanArgument forceBlitter;
    BooleanArgument useEvents;

    UsmFillSpecificPatternArguments()
        : memoryPlacement(*this, "memory", "Placement of the buffer"),
          bufferSize(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffer"),
          pattern(*this, "pattern", "The fill pattern represented hexadecimally, e.g. 0x91ABCD1254"),
          forceBlitter(*this, "forceBlitter", CommonHelpMessage::forceBlitter()),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct UsmFillSpecificPattern : TestCase<UsmFillSpecificPatternArguments> {
    using TestCase<UsmFillSpecificPatternArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmFillSpecificPattern";
    }

    std::string getHelp() const override {
        return "allocates a unified memory buffer and measures fill bandwidth. Allow specifying arbitrary pattern.";
    }
};
