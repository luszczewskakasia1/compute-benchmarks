#include "tests/ulls_best_submission_with_walker.h"

class UllsBestSubmissionWithWalkerTest : public ::testing::TestWithParam<Api> {
};

TEST_P(UllsBestSubmissionWithWalkerTest, Test) {
    UllsBestSubmissionWithWalker test{GetParam(), {}, 10};
    test.run();
}

INSTANTIATE_TEST_SUITE_P(
    UllsBestSubmissionWithWalkerTest,
    UllsBestSubmissionWithWalkerTest,
    ::testing::Values(Api::OpenCL, Api::L0));
