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
#include "framework/argument/enum/work_item_id_usage_argument.h"
#include "framework/test_case/test_case.h"

#include <sstream>

struct KernelSwitchLatencyArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument kernelCount;

    KernelSwitchLatencyArguments()
        : kernelCount(*this, "kernelCount", "Count of kernels") {}
};

struct KernelSwitchLatency : TestCase<KernelSwitchLatencyArguments> {
    using TestCase<KernelSwitchLatencyArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "KernelSwitchLatency";
    }

    std::string getHelp() const override {
        return "measures time from end of one kernel till start of next kernel";
    }
};

inline auto selectKernel(WorkItemIdUsage usedIds, const char *extension) {
    std::ostringstream result{};
    switch (usedIds) {
    case WorkItemIdUsage::None:
        result << "ulls_benchmark_write_one";
        break;
    case WorkItemIdUsage::Global:
        result << "ulls_benchmark_write_one_global_ids";
        break;
    case WorkItemIdUsage::Local:
        result << "ulls_benchmark_write_one_local_ids";
        break;
    default:
        FATAL_ERROR("Unknown work item id usage");
    }
    result << '.' << extension;
    return result.str();
}
