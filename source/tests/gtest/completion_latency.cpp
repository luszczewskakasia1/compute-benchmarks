#include "tests/completion_latency.h"

#include <gtest/gtest.h>

class CompletionLatencyTest : public ::testing::TestWithParam<Api> {
};

TEST_P(CompletionLatencyTest, Test) {
    UllsTest::CompletionLatencyArguments args{};
    args.api = GetParam();
    args.iterations = ::gtestIterations;

    UllsTest::CompletionLatency test{args};
    test.run();
}

INSTANTIATE_TEST_SUITE_P(
    CompletionLatencyTest,
    CompletionLatencyTest,
    ::testing::Values(Api::OpenCL, Api::L0));
