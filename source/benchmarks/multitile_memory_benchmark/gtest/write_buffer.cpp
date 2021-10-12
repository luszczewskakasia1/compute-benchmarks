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

#include "definitions/write_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<WriteBuffer> registerTestCase{};

class WriteBufferTest : public ::testing::TestWithParam<std::tuple<DeviceSelection, DeviceSelection, DeviceSelection, size_t, bool, bool>> {};

TEST_P(WriteBufferTest, Test) {
    WriteBufferArguments args;
    args.api = Api::OpenCL;
    args.contextPlacement = std::get<0>(GetParam());
    args.queuePlacement = std::get<1>(GetParam());
    args.bufferPlacement = std::get<2>(GetParam());
    args.size = std::get<3>(GetParam());
    args.compressed = std::get<4>(GetParam());
    args.useEvents = std::get<5>(GetParam());

    if (!args.validateArgumentsExtra()) {
        GTEST_SKIP(); // If above arguments make no sense (e.g. queue created outside of the context), skip the case
    }

    WriteBuffer test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    WriteBufferTest,
    WriteBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::contextDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true),
        ::testing::Values(false)));
