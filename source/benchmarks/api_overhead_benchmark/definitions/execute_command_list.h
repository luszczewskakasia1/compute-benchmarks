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

struct ExecuteCommandListArguments : TestCaseArgumentContainer {
    BooleanArgument useFence;
    BooleanArgument measureCompletionTime;

    ExecuteCommandListArguments()
        : useFence(*this, "UseFence", "Pass a non-null ze_fence_handle_t to the API call"),
          measureCompletionTime(*this, "measureCompletionTime", "Measures time taken to complete the submission (default is to measure only Execute call)") {}
};

struct ExecuteCommandList : TestCase<ExecuteCommandListArguments> {
    using TestCase<ExecuteCommandListArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "ExecuteCommandList";
    }

    std::string getHelp() const override {
        return "measures time spent in zeCommandQueueExecuteCommandLists on CPU.";
    }
};
