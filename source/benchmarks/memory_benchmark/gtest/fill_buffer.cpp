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

#include "definitions/fill_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<FillBuffer> registerTestCase{};

class FillBufferTest : public ::testing::TestWithParam<std::tuple<Api, size_t, BufferContents, size_t, bool, bool, bool>> {
};

TEST_P(FillBufferTest, Test) {
    FillBufferArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    args.contents = std::get<2>(GetParam());
    args.patternSize = std::get<3>(GetParam());
    args.compressed = std::get<4>(GetParam());
    args.forceBlitter = std::get<5>(GetParam());
    args.useEvents = std::get<6>(GetParam());

    FillBuffer test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    FillBufferTest,
    FillBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(BufferContents::Zeros),
        ::testing::Values(1, 16, 128),
        ::testing::Values(false, true),
        ::testing::Values(false, true),
        ::testing::Values(false)));
