#include "ulls_benchmark/definitions/best_walker_submission.h"

#include "framework/common_gtest_args.h"

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
    ::CommonGtestArgs::allApis());
