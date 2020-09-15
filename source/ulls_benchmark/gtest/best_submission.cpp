#include "ulls_benchmark/definitions/best_submission.h"

#include "framework/common_gtest_args.h"

#include <gtest/gtest.h>

class BestSubmissionTest : public ::testing::TestWithParam<Api> {
};

TEST_P(BestSubmissionTest, Test) {
    BestSubmissionArguments args{};
    args.api = GetParam();

    BestSubmission test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    BestSubmissionTest,
    BestSubmissionTest,
    ::CommonGtestArgs::allApis());
