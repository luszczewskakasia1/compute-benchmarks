#include "ulls_benchmark/definitions/completion_latency.h"

#include "framework/common_gtest_args.h"

#include <gtest/gtest.h>

class CompletionLatencyTest : public ::testing::TestWithParam<Api> {
};

TEST_P(CompletionLatencyTest, Test) {
    CompletionLatencyArguments args{};
    args.api = GetParam();

    CompletionLatency test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    CompletionLatencyTest,
    CompletionLatencyTest,
    ::CommonGtestArgs::allApis());
