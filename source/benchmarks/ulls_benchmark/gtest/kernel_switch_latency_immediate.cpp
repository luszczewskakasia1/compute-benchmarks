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

#include "definitions/kernel_switch_latency_immediate.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<KernelSwitchLatencyImmediate> registerTestCase{};

class KernelSwitchLatencyImmediateTest : public ::testing::TestWithParam<std::tuple<Api, size_t, bool, bool>> {
};

TEST_P(KernelSwitchLatencyImmediateTest, Test) {
    KernelSwitchLatencyImmediateArguments args;
    args.api = Api::L0;
    args.kernelCount = std::get<1>(GetParam());
    args.barrier = std::get<2>(GetParam());
    args.hostVisible = std::get<3>(GetParam());

    KernelSwitchLatencyImmediate test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    KernelSwitchLatencyImmediateTest,
    KernelSwitchLatencyImmediateTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(8, 16, 32),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
