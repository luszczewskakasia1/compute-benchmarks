#include "ulls_benchmark/definitions/round_trip_submission.h"

#include "framework/common_gtest_args.h"

#include <gtest/gtest.h>

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
