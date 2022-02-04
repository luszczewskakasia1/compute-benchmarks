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

#include "definitions/event_time.h"

#include "framework/test_case/register_test_case.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<EventTime> registerTestCase{};

class EventTimeTest : public ::testing::TestWithParam<std::tuple<bool, bool, EventScope, EventScope>> {
};

TEST_P(EventTimeTest, Test) {
    EventTimeArguments args{};
    args.api = Api::L0;
    args.useProfiling = std::get<0>(GetParam());
    args.hostVisible = std::get<1>(GetParam());
    args.signalScope = std::get<2>(GetParam());
    args.waitScope = std::get<3>(GetParam());

    EventTime test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    EventTimeTest,
    EventTimeTest,
    ::testing::Combine(
        ::testing::Values(false, true),
        ::testing::Values(false, true),
        ::testing::ValuesIn(EventScopeArgument::enumValues),
        ::testing::ValuesIn(EventScopeArgument::enumValues)));
