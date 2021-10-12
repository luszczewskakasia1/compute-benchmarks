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

#include "definitions/round_trip_submission.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<RoundTripSubmission> registerTestCase{};

class RoundTripSubmissionTest : public ::testing::TestWithParam<Api> {
};

TEST_P(RoundTripSubmissionTest, Test) {
    RoundTripSubmissionArguments args{};
    args.api = GetParam();

    RoundTripSubmission test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    RoundTripSubmissionTest,
    RoundTripSubmissionTest,
    ::CommonGtestArgs::allApis());
