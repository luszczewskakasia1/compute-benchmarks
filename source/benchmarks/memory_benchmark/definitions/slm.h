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

#include "framework/argument/compression_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct SlmTrafficArguments : TestCaseArgumentContainer {
    ByteSizeArgument size;
    PositiveIntegerArgument occupancyDivider;
    BooleanArgument writeDirection;

    SlmTrafficArguments()
        : size(*this, "size", "SLM Size"),
          occupancyDivider(*this, "occupancyDiv", "H/W load divider by 8, 4, 2, full occupancy"),
          writeDirection(*this, "direction", "write or read mode") {}
};

struct SlmTraffic : TestCase<SlmTrafficArguments> {
    using TestCase<SlmTrafficArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "SLM_DataAccessLatency";
    }

    std::string getHelp() const override {
        return "generates SLM local memory transactions inside thread group to measure latency between reads (uses Intel only private intel_get_cycle_counter() )";
    }
};
