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

#include "definitions/kernel_switch_latency.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<KernelSwitchLatency> registerTestCase{};

class KernelSwitchLatencyTest : public ::testing::TestWithParam<std::tuple<Api, size_t, bool>> {
};

TEST_P(KernelSwitchLatencyTest, Test) {
    KernelSwitchLatencyArguments args;
    args.api = std::get<0>(GetParam());
    args.kernelCount = std::get<1>(GetParam());
    args.flushBetweenEnqueues = std::get<2>(GetParam());

    KernelSwitchLatency test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    KernelSwitchLatencyTest,
    KernelSwitchLatencyTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(16, 32, 64, 128),
        ::testing::Values(false, true)));
