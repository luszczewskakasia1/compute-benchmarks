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

#include "definitions/unmap_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<UnmapBuffer> registerTestCase{};

class UnmapBufferTest : public ::testing::TestWithParam<std::tuple<Api, size_t, BufferContents, bool, MapFlags, bool>> {
};

TEST_P(UnmapBufferTest, Test) {
    UnmapBufferArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    args.contents = std::get<2>(GetParam());
    args.compressed = std::get<3>(GetParam());
    args.mapFlags = std::get<4>(GetParam());
    args.useEvents = std::get<5>(GetParam());

    UnmapBuffer test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    UnmapBufferTest,
    UnmapBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(BufferContents::Zeros),
        ::testing::Values(false, true),
        ::testing::Values(MapFlags::Read, MapFlags::Write, MapFlags::WriteInvalidate),
        ::testing::Values(false)));
