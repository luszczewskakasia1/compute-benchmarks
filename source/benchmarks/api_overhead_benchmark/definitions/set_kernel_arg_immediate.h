/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2022 Intel Corporation
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

struct KernelSetArgumentValueImmediateArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument argumentSize;

    KernelSetArgumentValueImmediateArguments()
        : argumentSize(*this, "argSize", "Kernel argument size in bytes") {}
};

struct KernelSetArgumentValueImmediate : TestCase<KernelSetArgumentValueImmediateArguments> {
    using TestCase<KernelSetArgumentValueImmediateArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "KernelSetArgumentValueImmediate";
    }

    std::string getHelp() const override {
        return "measures time spent in zeKernelSetArgumentValue for immediate arguments on CPU.";
    }
};
