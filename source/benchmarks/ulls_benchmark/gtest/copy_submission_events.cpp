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

#include "definitions/copy_submission_events.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<CopySubmissionEvents> registerTestCase{};

class CopySubmissionEventsTest : public ::testing::TestWithParam<std::tuple<Api, Engine>> {
};

TEST_P(CopySubmissionEventsTest, Test) {
    CopySubmissionEventsArguments args{};
    args.api = std::get<0>(GetParam());
    args.engine = std::get<1>(GetParam());

    CopySubmissionEvents test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    CopySubmissionEventsTest,
    CopySubmissionEventsTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(Engine::Ccs0, Engine::Ccs1, Engine::Ccs2, Engine::Ccs3, Engine::Bcs, Engine::Bcs1, Engine::Bcs2, Engine::Bcs3, Engine::Bcs4, Engine::Bcs5, Engine::Bcs6, Engine::Bcs7, Engine::Bcs8)));
