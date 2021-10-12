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

#include "definitions/read_buffer_misaligned.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<ReadBufferMisaligned> registerTestCase{};

class ReadBufferMisalignedTest : public ::testing::TestWithParam<std::tuple<Api, size_t, size_t, bool>> {
};

TEST_P(ReadBufferMisalignedTest, Test) {
    ReadBufferMisalignedArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    args.misalignmentFromCacheline = std::get<2>(GetParam());
    args.useEvents = std::get<3>(GetParam());

    ReadBufferMisaligned test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    ReadBufferMisalignedTest,
    ReadBufferMisalignedTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(16 * megaByte),
        ::testing::Values(0, 1, 2, 4),
        ::testing::Values(false)));
