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

#include "definitions/stream_memory.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<StreamMemory> registerTestCase{};

class StreamMemoryTest : public ::testing::TestWithParam<std::tuple<Api, StreamMemoryType, size_t, bool>> {
};

TEST_P(StreamMemoryTest, Test) {
    StreamMemoryArguments args;
    args.api = std::get<0>(GetParam());
    args.type = std::get<1>(GetParam());
    args.size = std::get<2>(GetParam());
    args.useEvents = std::get<3>(GetParam());

    StreamMemory test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    StreamMemoryTest,
    StreamMemoryTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::ValuesIn(StreamMemoryTypeArgument::enumValues),
        ::testing::Values(512 * megaByte),
        ::testing::Values(false, true)));
