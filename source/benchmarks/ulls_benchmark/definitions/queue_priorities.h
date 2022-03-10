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
#include "framework/test_case/test_case.h"

struct QueuePrioritiesArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument kernelTime;
    BooleanArgument usePriorities;
    PositiveIntegerArgument workgroupCount;

    QueuePrioritiesArguments()
        : kernelTime(*this, "kernelTime", "How long each work item is in low priority kernel"),
          usePriorities(*this, "priorities", "Low priority command queue property is used"),
          workgroupCount(*this, "wgc", "Workgroup count of high priority kernel") {}
};

struct QueuePriorities : TestCase<QueuePrioritiesArguments> {
    using TestCase<QueuePrioritiesArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "QueuePriorities";
    }

    std::string getHelp() const override {
        return "Uses queues with different priorities to meassure submission and context switch latencies";
    }
};
