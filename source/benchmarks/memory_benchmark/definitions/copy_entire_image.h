#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_memory_placement.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_three_component_uint.h"
#include "framework/utility/common_help_message.h"

struct CopyEntireImageArguments : Arguments {
    ThreeComponentSizeArgument size;
    BooleanArgument forceBlitter;
    BooleanArgument useEvents;

    CopyEntireImageArguments()
        : size(*this, "size", "Size of the image"),
          forceBlitter(*this, "forceBlitter", CommonHelpMessage::forceBlitter()),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}
};

struct CopyEntireImage : TestCase<CopyEntireImageArguments> {
    using TestCase<CopyEntireImageArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "CopyEntireImage";
    }

    std::string getHelp() const override {
        return "allocates two image objects and measures copy bandwidth between them. Images will "
               "be placed in device memory, if it's available.";
    }
};
