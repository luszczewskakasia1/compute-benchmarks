#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_device_selection.h"
#include "framework/test_case_argument/enum/test_case_argument_multi_device_selection.h"
#include "framework/test_case_argument/test_case_argument_compression.h"
#include "framework/utility/common_help_message.h"

struct FillBufferArguments : TestCaseArguments {
    MultiDeviceSelectionTestCaseArgument contextPlacement;
    DeviceSelectionTestCaseArgument queuePlacement;
    DeviceSelectionTestCaseArgument bufferPlacement;
    ByteSizeTestCaseArgument size;
    ByteSizeTestCaseArgument patternSize;
    CompressionBooleanTestCaseArgument compressed;
    BooleanTestCaseArgument forceBlitter;
    BooleanTestCaseArgument useEvents;

    FillBufferArguments()
        : contextPlacement(*this, "context", "How context will be created"),
          queuePlacement(*this, "queue", "Which device within the context will perform the operation"),
          bufferPlacement(*this, "memory", "Placement of memory for the buffer"),
          size(*this, "size", "Size of the buffer"),
          patternSize(*this, "patternSize", "Size of the fill pattern"),
          compressed(*this, "compressed", CommonHelpMessage::compression("buffer")),
          forceBlitter(*this, "forceBlitter", CommonHelpMessage::forceBlitter()),
          useEvents(*this, "useEvents", CommonHelpMessage::useEvents()) {}

    bool validateArgumentsExtra() const override {
        return DeviceSelectionHelper::isSubset(contextPlacement, queuePlacement) &&
               DeviceSelectionHelper::isSubset(contextPlacement, bufferPlacement);
    }
};

struct FillBuffer : TestCase<FillBufferArguments> {
    using TestCase<FillBufferArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "FillBuffer";
    }

    std::string getHelp() const override {
        return "allocates an OpenCL buffer and measures fill bandwidth. Buffer will be placed in "
               "device memory, if it's available.";
    }
};
