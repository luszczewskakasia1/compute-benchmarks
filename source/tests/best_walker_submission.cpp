#include "tests/best_walker_submission.h"

class BestWalkerSubmissionTest : public ::testing::TestWithParam<Api> {
};

TEST_P(BestWalkerSubmissionTest, Test) {
    UllsTest::BestWalkerSubmissionArguments args{};
    args.api = GetParam();
    args.iterations = ::gtestIterations;

    UllsTest::BestWalkerSubmission test{args};
    test.run();
}

INSTANTIATE_TEST_SUITE_P(
    BestWalkerSubmissionTest,
    BestWalkerSubmissionTest,
    ::testing::Values(Api::OpenCL, Api::L0));
