#include "ulls_benchmark/start_latency.h"

#include <gtest/gtest.h>

class StartLatencyTest : public ::testing::TestWithParam<Api> {
};

TEST_P(StartLatencyTest, Test) {
    StartLatencyArguments args{};
    args.api = GetParam();

    StartLatency test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    StartLatencyTest,
    StartLatencyTest,
    ::testing::Values(Api::OpenCL, Api::L0));
