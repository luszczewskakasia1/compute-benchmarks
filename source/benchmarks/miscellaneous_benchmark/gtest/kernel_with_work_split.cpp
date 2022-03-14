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

#include "definitions/kernel_with_work_split.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<KernelWithWorkSplit> registerTestCase{};

class KernelWithWorkSplitSubmissionTest : public ::testing::TestWithParam<std::tuple<Api, WorkItemIdUsage, size_t, size_t, size_t>> {
};

TEST_P(KernelWithWorkSplitSubmissionTest, Test) {
    KernelWithWorkArgumentsSplit args;
    args.api = std::get<0>(GetParam());
    args.usedIds = std::get<1>(GetParam());
    args.workgroupCount = std::get<2>(GetParam());
    args.workgroupSize = std::get<3>(GetParam());
    args.splitSize = std::get<4>(GetParam());

    KernelWithWorkSplit test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    KernelWithWorkSplitSubmissionTest,
    KernelWithWorkSplitSubmissionTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(WorkItemIdUsage::None, WorkItemIdUsage::Global, WorkItemIdUsage::Local),
        ::testing::Values(128, 1024, 8096),
        ::CommonGtestArgs::workgroupSize(),
        ::testing::Values(1u, 2u, 4u, 8u, 16u, 32u, 64u)));
