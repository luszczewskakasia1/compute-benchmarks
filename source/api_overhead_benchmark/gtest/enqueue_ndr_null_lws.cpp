#include "api_overhead_benchmark/definitions/enqueue_ndr_null_lws.h"

#include <gtest/gtest.h>

class EnqueueNdrNullLwsTest : public ::testing::TestWithParam<std::tuple<size_t, bool, bool>> {
};

TEST_P(EnqueueNdrNullLwsTest, Test) {
    EnqueueNdrNullLwsArguments args{};
    args.api = Api::OpenCL;
    args.gws = std::get<0>(GetParam());
    args.useProfiling = std::get<1>(GetParam());
    args.useEvent = std::get<2>(GetParam());

    EnqueueNdrNullLws test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    EnqueueNdrNullLwsTest,
    EnqueueNdrNullLwsTest,
    ::testing::Combine(
        ::testing::Values(1, 100, 1000),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
