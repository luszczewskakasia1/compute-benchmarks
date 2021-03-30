#pragma once

#include "framework/argument/basic_argument.h"
#include "framework/argument/bitmap_argument.h"
#include "framework/argument/enum/usm_memory_placement_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

using BcsBitmaskArgument = BitmaskArgument<9, false>;

struct SimultaneousBlitterCopiesArguments : TestCaseArgumentContainer {
    UsmMemoryPlacementArgument sourcePlacement;
    UsmMemoryPlacementArgument destinationPlacement;
    ByteSizeArgument size;
    BcsBitmaskArgument blitters;

    SimultaneousBlitterCopiesArguments()
        : sourcePlacement(*this, "src", "Placement of the source buffer"),
          destinationPlacement(*this, "dst", "Placement of the destination buffer"),
          size(*this, "size", "Size of the buffer"),
          blitters(*this, "blitters", "A bit mask for selecting copy engines") {}
};

struct SimultaneousBlits : TestCase<SimultaneousBlitterCopiesArguments> {
    using TestCase<SimultaneousBlitterCopiesArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "SimultaneousBlits";
    }

    std::string getHelp() const override {
        return "allocates two unified shared memory buffers and measures copy bandwidth between them.";
    }
};
