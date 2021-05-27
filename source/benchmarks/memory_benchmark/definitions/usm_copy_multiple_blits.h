#pragma once

#include "framework/argument/basic_argument.h"
#include "framework/argument/bitmap_argument.h"
#include "framework/argument/enum/usm_memory_placement_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

using BcsBitmaskArgument = BitmaskArgument<9, false>;

struct UsmCopyMultipleBlitsArguments : TestCaseArgumentContainer {
    UsmMemoryPlacementArgument sourcePlacement;
    UsmMemoryPlacementArgument destinationPlacement;
    ByteSizeArgument size;
    BcsBitmaskArgument blitters;

    UsmCopyMultipleBlitsArguments()
        : sourcePlacement(*this, "src", "Placement of the source buffer"),
          destinationPlacement(*this, "dst", "Placement of the destination buffer"),
          size(*this, "size", "Size of the operation processed by each engine"),
          blitters(*this, "blitters", "A bit mask for selecting copy engines") {}
};

struct UsmCopyMultipleBlits : TestCase<UsmCopyMultipleBlitsArguments> {
    using TestCase<UsmCopyMultipleBlitsArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmCopyMultipleBlits";
    }

    std::string getHelp() const override {
        return "allocates two unified shared memory buffers for each enabled blitter and measures "
               "copy bandwidth between them. Results for each individual blitter engine is "
               "measured using GPU-based timings and reported separately. Total bandwidths are "
               "calculated by dividing the sum of sizes of all operation by the worst time among "
               "all engines.";
    }
};
