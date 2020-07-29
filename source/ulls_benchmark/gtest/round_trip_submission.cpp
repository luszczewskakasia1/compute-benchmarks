#include "ulls_benchmark/round_trip_submission.h"

#include <gtest/gtest.h>

class RoundTripSubmissionTest : public ::testing::TestWithParam<Api> {
};

TEST_P(RoundTripSubmissionTest, Test) {
    UllsTest::RoundTripSubmissionArguments args{};
    args.api = GetParam();

    UllsTest::RoundTripSubmission test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    RoundTripSubmissionTest,
    RoundTripSubmissionTest,
    ::testing::Values(Api::OpenCL, Api::L0));
