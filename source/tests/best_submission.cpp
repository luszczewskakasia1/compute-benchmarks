#include "tests/best_submission.h"

class BestrSubmissionTest : public ::testing::TestWithParam<Api> {
};

TEST_P(BestrSubmissionTest, Test) {
    UllsTest::BestSubmissionArguments args{};
    args.iterations = 10;

    UllsTest::BestSubmission test{GetParam(), args};
    test.run();
}

INSTANTIATE_TEST_SUITE_P(
    BestrSubmissionTest,
    BestrSubmissionTest,
    ::testing::Values(Api::OpenCL, Api::L0));
