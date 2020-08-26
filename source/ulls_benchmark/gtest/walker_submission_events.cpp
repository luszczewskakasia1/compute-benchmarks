#include "ulls_benchmark/definitions/walker_submission_events.h"

#include <gtest/gtest.h>

class WalkerSubmissionEventsTest : public ::testing::TestWithParam<Api> {
};

TEST_P(WalkerSubmissionEventsTest, Test) {
    WalkerSubmissionEventsArguments args{};
    args.api = GetParam();

    WalkerSubmissionEvents test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    WalkerSubmissionEventsTest,
    WalkerSubmissionEventsTest,
    ::testing::Values(Api::OpenCL, Api::L0));
