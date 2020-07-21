#include "tests/round_trip_submission.h"

class RoundTripSubmissionTest : public ::testing::TestWithParam<Api> {
};

TEST_P(RoundTripSubmissionTest, Test) {
    UllsTest::RoundTripSubmissionArguments args{};
    args.iterations = 10;

    UllsTest::RoundTripSubmission test{GetParam(), args};
    test.run();
}

INSTANTIATE_TEST_SUITE_P(
    RoundTripSubmissionTest,
    RoundTripSubmissionTest,
    ::testing::Values(Api::OpenCL, Api::L0));
