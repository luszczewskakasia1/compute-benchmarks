#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_buffer_contents.h"
#include "framework/test_case_argument/enum/test_case_argument_memory_placement.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct UsmCopyArguments : TestCaseArguments {
    MemoryPlacementTestCaseArgument sourcePlacement;
    MemoryPlacementTestCaseArgument destinationPlacement;
    ByteSizeTestCaseArgument size;
    BufferContentsTestCaseArgument contents;
    BooleanTestCaseArgument forceBlitter;
    BooleanTestCaseArgument useEvents;

    UsmCopyArguments()
        : sourcePlacement(*this, "src", "Placement of the source buffer"),
          destinationPlacement(*this, "dst", "Placement of the destination buffer"),
          size(*this, "size", "Size of the buffer"),
          contents(*this, "contents", "Contents of the buffers"),
          forceBlitter(*this, "forceBlitter", CommonHelpMessage::forceBlitter()),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct UsmCopy : TestCase<UsmCopyArguments> {
    using TestCase<UsmCopyArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmCopy";
    }

    std::string getHelp() const override {
        return "allocates two unified shared memory buffers and measures copy bandwidth between them.";
    }
};
