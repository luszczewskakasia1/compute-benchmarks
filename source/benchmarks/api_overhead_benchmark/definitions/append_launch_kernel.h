/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
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
#include "framework/test_case/test_case.h"

struct AppendLaunchKernelArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument workgroupCount;
    IntegerArgument workgroupSize;
    BooleanArgument useEvent;

    AppendLaunchKernelArguments()
        : workgroupCount(*this, "wgc", "Workgroup count"),
          workgroupSize(*this, "wgs", "Workgroup size, pass 0 to make the driver calculate it during enqueue"),
          useEvent(*this, "event", "Pass output event to the enqueue call") {}
};

struct AppendLaunchKernel : TestCase<AppendLaunchKernelArguments> {
    using TestCase<AppendLaunchKernelArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "AppendLaunchKernel";
    }

    std::string getHelp() const override {
        return "measures time spent in zeCommandListAppendLaunchKernel on CPU.";
    }
};
