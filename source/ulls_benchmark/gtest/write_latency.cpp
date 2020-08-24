#include "ulls_benchmark/write_latency.h"

#include <gtest/gtest.h>

class WriteLatencyTest : public ::testing::TestWithParam<Api> {
};

TEST_P(WriteLatencyTest, Test) {
    WriteLatencyArguments args{};
    args.api = GetParam();

    WriteLatency test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    WriteLatencyTest,
    WriteLatencyTest,
    ::testing::Values(Api::OpenCL, Api::L0));
