#pragma once

#include "framework/argument/basic_argument.h"
#include "framework/argument/enum/work_item_id_usage_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct BarrierBetweenKernelsArguments : TestCaseArgumentContainer {
    WorkItemIdUsageArgument usedIds;
    PositiveIntegerArgument measuredCommands;
    PositiveIntegerArgument workgroupCount;
    PositiveIntegerArgument workgroupSize;

    BarrierBetweenKernelsArguments()
        : usedIds(*this, "usedIds", "Which of the get_global_id() and get_local_id() calls will be used in the kernel"),
          measuredCommands(*this, "measuredCommands", CommonHelpMessage::measuredCommandsCount()),
          workgroupCount(*this, "wgc", "Workgroup count"),
          workgroupSize(*this, "wgs", "Workgroup size (aka local work size)") {}
};

struct BarrierBetweenKernels : TestCase<BarrierBetweenKernelsArguments> {

    using TestCase<BarrierBetweenKernelsArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "BarrierBetweenKernels";
    }

    std::string getHelp() const override {
        return "measures time required to run a barrier command between 2 kernels.";
    }
};
