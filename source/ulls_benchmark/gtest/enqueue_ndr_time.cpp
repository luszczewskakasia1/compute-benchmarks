#include "ulls_benchmark/definitions/enqueue_ndr_time.h"

#include <gtest/gtest.h>

class EnqueueNdrTimeTest : public ::testing::TestWithParam<std::tuple<size_t, size_t, bool, bool>> {
};

TEST_P(EnqueueNdrTimeTest, Test) {
    EnqueueNdrTimeArguments args{};
    args.api = Api::OpenCL;
    args.workgroupCount = std::get<0>(GetParam());
    args.workgroupSize = std::get<1>(GetParam());
    args.useProfiling = std::get<2>(GetParam());
    args.useEvent = std::get<3>(GetParam());

    EnqueueNdrTime test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    EnqueueNdrTimeTest,
    EnqueueNdrTimeTest,
    ::testing::Combine(
        ::testing::Values(1, 100, 1000),
        ::testing::Values(0, 1, 32, 256),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
