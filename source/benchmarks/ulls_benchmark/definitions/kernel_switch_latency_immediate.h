/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#pragma once

#include "framework/argument/basic_argument.h"
#include "framework/argument/enum/work_item_id_usage_argument.h"
#include "framework/test_case/test_case.h"

#include <sstream>

struct KernelSwitchLatencyImmediateArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument kernelCount;
    BooleanArgument barrier;
    BooleanArgument hostVisible;

    KernelSwitchLatencyImmediateArguments()
        : kernelCount(*this, "kernelCount", "Count of kernels"),
          barrier(*this, "barrier", "synchronization with barrier instead of events"),
          hostVisible(*this, "hostVisible", "events are with host visible flag") {}
};

struct KernelSwitchLatencyImmediate : TestCase<KernelSwitchLatencyImmediateArguments> {
    using TestCase<KernelSwitchLatencyImmediateArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "KernelSwitchLatencyImmediate";
    }

    std::string getHelp() const override {
        return "measures time from end of one kernel till start of next kernel using immediate command lists";
    }
};
