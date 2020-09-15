#include "ulls_benchmark/definitions/write_latency.h"

#include "framework/common_gtest_args.h"

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
    ::CommonGtestArgs::allApis());
