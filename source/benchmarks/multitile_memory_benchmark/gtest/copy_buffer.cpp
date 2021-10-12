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

#include "definitions/copy_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<CopyBuffer> registerTestCase{};

class CopyBufferTest : public ::testing::TestWithParam<std::tuple<DeviceSelection, DeviceSelection, DeviceSelection, DeviceSelection, size_t, bool, bool, bool>> {};

TEST_P(CopyBufferTest, Test) {
    CopyBufferArguments args;
    args.api = Api::OpenCL;
    args.contextPlacement = std::get<0>(GetParam());
    args.queuePlacement = std::get<1>(GetParam());
    args.srcPlacement = std::get<2>(GetParam());
    args.dstPlacement = std::get<3>(GetParam());
    args.size = std::get<4>(GetParam());
    args.srcCompressed = std::get<5>(GetParam());
    args.dstCompressed = std::get<6>(GetParam());
    args.useEvents = std::get<7>(GetParam());

    if (!args.validateArgumentsExtra()) {
        GTEST_SKIP(); // If above arguments make no sense (e.g. queue created outside of the context), skip the case
    }

    CopyBuffer test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    CopyBufferTest,
    CopyBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::contextDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true),
        ::testing::Values(false, true),
        ::testing::Values(false)));
