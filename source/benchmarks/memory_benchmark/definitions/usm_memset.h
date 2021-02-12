#pragma once

#include "framework/argument/basic_argument.h"
#include "framework/argument/enum/buffer_contents_argument.h"
#include "framework/argument/enum/memory_placement_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct UsmMemsetArguments : TestCaseArgumentContainer {
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
