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

struct EnqueueNdrTimeArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument workgroupCount;
    PositiveIntegerArgument workgroupSize;
    BooleanArgument useOoq;
    BooleanArgument useProfiling;
    BooleanArgument useEvent;

    EnqueueNdrTimeArguments()
        : workgroupCount(*this, "wgc", "Workgroup count"),
          workgroupSize(*this, "wgs", "Workgroup size"),
          useOoq(*this, "ooq", "Use out of order queue"),
          useProfiling(*this, "profiling", "Creating a profiling queue"),
          useEvent(*this, "event", "Pass output event to the enqueue call") {}
};

struct EnqueueNdrTime : TestCase<EnqueueNdrTimeArguments> {
    using TestCase<EnqueueNdrTimeArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "EnqueueNdrTime";
    }

    std::string getHelp() const override {
        return "measures time spent in clEnqueueNDRangeKernel on CPU.";
    }
};
