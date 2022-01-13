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
#include "framework/argument/enum/engine_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct EventCtxtSwitchLatencyArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument measuredCommands;
    EngineArgument firstEngine;
    EngineArgument secondEngine;

    EventCtxtSwitchLatencyArguments()
        : measuredCommands(*this, "measuredCommands", CommonHelpMessage::measuredCommandsCount()),
          firstEngine(*this, "firstEngine", "first engine to measure context switch latency"),
          secondEngine(*this, "secondEngine", "second engine to measure context switch latency") {}
};

struct EventCtxtSwitchLatency : TestCase<EventCtxtSwitchLatencyArguments> {
    using TestCase<EventCtxtSwitchLatencyArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "EventCtxtSwitchLatency";
    }

    std::string getHelp() const override {
        return "measures context switching latency time required to switch between various engine types";
    }
};
