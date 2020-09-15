#include "ulls_benchmark/definitions/walker_completion_latency.h"

#include "framework/common_gtest_args.h"

#include <gtest/gtest.h>

class WalkerCompletionLatencyTest : public ::testing::TestWithParam<Api> {
};

TEST_P(WalkerCompletionLatencyTest, Test) {
    WalkerCompletionLatencyArguments args{};
    args.api = GetParam();

    WalkerCompletionLatency test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    WalkerCompletionLatencyTest,
    WalkerCompletionLatencyTest,
    ::CommonGtestArgs::allApis());
