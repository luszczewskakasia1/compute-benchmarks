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

#include "definitions/flush_time.h"

#include "framework/test_case/register_test_case.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<FlushTime> registerTestCase{};

class FlushTimeTest : public ::testing::TestWithParam<std::tuple<size_t, size_t, bool, bool>> {
};

TEST_P(FlushTimeTest, Test) {
    FlushTimeArguments args{};
    args.api = Api::OpenCL;
    args.workgroupCount = std::get<0>(GetParam());
    args.workgroupSize = std::get<1>(GetParam());
    args.useOoq = std::get<2>(GetParam());
    args.useEvent = std::get<3>(GetParam());

    FlushTime test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    FlushTimeTest,
    FlushTimeTest,
    ::testing::Combine(
        ::testing::Values(1, 100, 1000),
        ::testing::Values(0, 1, 32, 256),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
