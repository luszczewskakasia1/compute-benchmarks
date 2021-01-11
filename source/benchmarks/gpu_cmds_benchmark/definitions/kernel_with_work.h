#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_work_item_id_usage.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct KernelWithWorkArguments : TestCaseArguments {
    WorkItemIdUsageTestCaseArgument usedIds;
    PositiveIntegerTestCaseArgument measuredCommands;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    KernelWithWorkArguments()
        : usedIds(*this, "usedIds", "Which of the get_global_id() and get_local_id() calls will be used in the kernel"),
          measuredCommands(*this, "measuredCommands", CommonHelpMessage::measuredCommandsCount()),
          workgroupCount(*this, "wgc", "Workgroup count"),
          workgroupSize(*this, "wgs", "Workgroup size (aka local work size)") {}
};

struct KernelWithWork : TestCase<KernelWithWorkArguments> {

    using TestCase<KernelWithWorkArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "KernelWithWork";
    }

    std::string getHelp() const override {
        return "measures time required to run a GPU kernel which assigns values to elements of a buffer.";
    }
};
