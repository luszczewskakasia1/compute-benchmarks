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

struct DestroyCommandListArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument cmdListCount;

    DestroyCommandListArguments()
        : cmdListCount(*this, "CmdListCount", "Number of cmdlists to destroy") {}
};

struct DestroyCommandList : TestCase<DestroyCommandListArguments> {
    using TestCase<DestroyCommandListArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "DestroyCommandList";
    }

    std::string getHelp() const override {
        return "measures time spent in zeCommandListDestroy on CPU.";
    }
};
