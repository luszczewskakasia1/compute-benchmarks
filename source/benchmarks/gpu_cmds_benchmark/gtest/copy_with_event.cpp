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

#include "definitions/copy_with_event.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<CopyWithEvent> registerTestCase{};

class CopyWithEventTest : public ::testing::TestWithParam<std::tuple<size_t, bool, bool, bool>> {
};

TEST_P(CopyWithEventTest, Test) {
    CopyWithEventArguments args{};

    args.api = Api::L0;
    args.measuredCommands = std::get<0>(GetParam());
    args.useHostSignalEvent = std::get<1>(GetParam());
    args.useDeviceWaitEvent = std::get<2>(GetParam());
    args.useTimestampEvent = std::get<3>(GetParam());

    CopyWithEvent test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    CopyWithEventTest,
    CopyWithEventTest,
    ::testing::Combine(
        ::testing::Values(500),
        ::testing::Values(false, true),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
