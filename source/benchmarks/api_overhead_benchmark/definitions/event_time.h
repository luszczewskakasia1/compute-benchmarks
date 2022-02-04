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
#include "framework/argument/enum/event_scope_argument.h"
#include "framework/test_case/test_case.h"

struct EventTimeArguments : TestCaseArgumentContainer {
    BooleanArgument useProfiling;
    BooleanArgument hostVisible;
    EventScopeArgument signalScope;
    EventScopeArgument waitScope;

    EventTimeArguments()
        : useProfiling(*this, "useProfiling", "Event will use profiling"),
          hostVisible(*this, "hostVisible", "Event will set host visible flag"),
          signalScope(*this, "signal", "Type of signal scope"),
          waitScope(*this, "wait", "Type of wait scope") {}
};

struct EventTime : TestCase<EventTimeArguments> {
    using TestCase<EventTimeArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "EventCreation";
    }

    std::string getHelp() const override {
        return "measures time spent to create event";
    }
};
