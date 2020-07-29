#include "ulls_benchmark/completion_latency.h"

#include <gtest/gtest.h>

class CompletionLatencyTest : public ::testing::TestWithParam<Api> {
};

TEST_P(CompletionLatencyTest, Test) {
    UllsTest::CompletionLatencyArguments args{};
    args.api = GetParam();

    UllsTest::CompletionLatency test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    CompletionLatencyTest,
    CompletionLatencyTest,
    ::testing::Values(Api::OpenCL, Api::L0));
