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

#include "framework/test_case/test_case.h"

struct CompletionLatencyArguments : TestCaseArgumentContainer {};

struct CompletionLatency : TestCase<CompletionLatencyArguments> {
    using TestCase<CompletionLatencyArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "CompletionLatency";
    }

    std::string getHelp() const override {
        return "enqueues system memory write and measures time between the moment, when update is "
               "visible on CPU and the moment, when synchronizing call returns.";
    }
};
