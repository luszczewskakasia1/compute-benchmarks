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

#include "definitions/queue_priorities.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<QueuePriorities> registerTestCase{};

class QueuePrioritiesTest : public ::testing::TestWithParam<std::tuple<Api, size_t, bool, size_t>> {
};

TEST_P(QueuePrioritiesTest, Test) {
    QueuePrioritiesArguments args{};
    args.api = std::get<0>(GetParam());
    args.kernelTime = std::get<1>(GetParam());
    args.usePriorities = std::get<2>(GetParam());
    args.workgroupCount = std::get<3>(GetParam());

    QueuePriorities test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    QueuePrioritiesTest,
    QueuePrioritiesTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(1, 10, 100, 1000, 10000),
        ::testing::Values(false, true),
        ::testing::Values(1, 10, 100, 1000, 10000)));
