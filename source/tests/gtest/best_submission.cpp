#include "tests/best_submission.h"

#include <gtest/gtest.h>

class BestrSubmissionTest : public ::testing::TestWithParam<Api> {
};

TEST_P(BestrSubmissionTest, Test) {
    UllsTest::BestSubmissionArguments args{};
    args.api = GetParam();

    UllsTest::BestSubmission test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    BestrSubmissionTest,
    BestrSubmissionTest,
    ::testing::Values(Api::OpenCL, Api::L0));
