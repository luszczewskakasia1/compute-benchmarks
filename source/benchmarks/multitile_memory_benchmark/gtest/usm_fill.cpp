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

#include "definitions/usm_fill.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<UsmFill> registerTestCase{};

class UsmFillTest : public ::testing::TestWithParam<std::tuple<Api, DeviceSelection, DeviceSelection, DeviceSelection, size_t, size_t, bool, bool>> {};

TEST_P(UsmFillTest, Test) {
    UsmFillArguments args;
    args.api = std::get<0>(GetParam());
    args.contextPlacement = std::get<1>(GetParam());
    args.queuePlacement = std::get<2>(GetParam());
    args.bufferPlacement = std::get<3>(GetParam());
    args.size = std::get<4>(GetParam());
    args.patternSize = std::get<5>(GetParam());
    args.forceBlitter = std::get<6>(GetParam());
    args.useEvents = std::get<7>(GetParam());

    if (!args.validateArgumentsExtra()) {
        GTEST_SKIP(); // If above arguments make no sense (e.g. queue created outside of the context), skip the case
    }

    UsmFill test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    UsmFillTest,
    UsmFillTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::CommonGtestArgs::contextDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::CommonGtestArgs::usmDeviceSelections(),
        ::testing::Values(512 * megaByte),
        ::testing::Values(1, 128),
        ::testing::Values(false, true),
        ::testing::Values(false)));
