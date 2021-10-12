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

#include "definitions/enqueue_ndr_null_lws.h"

#include "framework/test_case/register_test_case.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<EnqueueNdrNullLws> registerTestCase{};

class EnqueueNdrNullLwsTest : public ::testing::TestWithParam<std::tuple<size_t, bool, bool, bool>> {
};

TEST_P(EnqueueNdrNullLwsTest, Test) {
    EnqueueNdrNullLwsArguments args{};
    args.api = Api::OpenCL;
    args.gws = std::get<0>(GetParam());
    args.useOoq = std::get<1>(GetParam());
    args.useProfiling = std::get<2>(GetParam());
    args.useEvent = std::get<3>(GetParam());

    EnqueueNdrNullLws test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    EnqueueNdrNullLwsTest,
    EnqueueNdrNullLwsTest,
    ::testing::Combine(
        ::testing::Values(1, 100, 1000),
        ::testing::Values(false, true),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
