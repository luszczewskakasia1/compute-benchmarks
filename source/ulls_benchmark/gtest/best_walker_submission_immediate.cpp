#include "ulls_benchmark/best_walker_submission_immediate.h"

#include <gtest/gtest.h>

class BestWalkerSubmissionImmediateTest : public ::testing::TestWithParam<Api> {
};

TEST_P(BestWalkerSubmissionImmediateTest, Test) {
    BestWalkerSubmissionImmediateArguments args{};
    args.api = GetParam();

    BestWalkerSubmissionImmediate test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    BestWalkerSubmissionImmediateTest,
    BestWalkerSubmissionImmediateTest,
    ::testing::Values(Api::L0));
