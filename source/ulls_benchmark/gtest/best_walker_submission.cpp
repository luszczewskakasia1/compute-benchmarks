#include "ulls_benchmark/best_walker_submission.h"

#include <gtest/gtest.h>

class BestWalkerSubmissionTest : public ::testing::TestWithParam<Api> {
};

TEST_P(BestWalkerSubmissionTest, Test) {
    BestWalkerSubmissionArguments args{};
    args.api = GetParam();

    BestWalkerSubmission test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    BestWalkerSubmissionTest,
    BestWalkerSubmissionTest,
    ::testing::Values(Api::OpenCL, Api::L0));
