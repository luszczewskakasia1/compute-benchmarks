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

#include "definitions/multi_queue_submission.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<MultiQueueSubmission> registerTestCase{};

class MultiQueueSubmissionTest : public ::testing::TestWithParam<std::tuple<Api, size_t, size_t, size_t>> {
};

TEST_P(MultiQueueSubmissionTest, Test) {
    MultiQueueSubmissionArguments args{};
    args.api = std::get<0>(GetParam());
    args.queueCount = std::get<1>(GetParam());
    args.workgroupCount = std::get<2>(GetParam());
    args.workgroupSize = std::get<3>(GetParam());

    MultiQueueSubmission test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    MultiQueueSubmissionTest,
    MultiQueueSubmissionTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(2, 4, 8, 16, 32, 64, 128),
        CommonGtestArgs::workgroupCount(),
        CommonGtestArgs::workgroupSize()));