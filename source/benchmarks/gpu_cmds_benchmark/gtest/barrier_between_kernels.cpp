#include "definitions/barrier_between_kernels.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<BarrierBetweenKernels> registerTestCase{};

class BarrierBetweenKernelsTest : public ::testing::TestWithParam<std::tuple<size_t, size_t>> {
};

TEST_P(BarrierBetweenKernelsTest, Test) {
    BarrierBetweenKernelsArguments args{};
    args.api = Api::L0;
    args.workgroupCount = std::get<0>(GetParam());
    args.workgroupSize = std::get<1>(GetParam());

    BarrierBetweenKernels test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    BarrierBetweenKernelsTest,
    BarrierBetweenKernelsTest,
    ::testing::Combine(
        ::testing::Values(1, 100, 1000, 10000, 20000, 40000, 60000, 80000, 100000),
        ::CommonGtestArgs::workgroupSize()));
