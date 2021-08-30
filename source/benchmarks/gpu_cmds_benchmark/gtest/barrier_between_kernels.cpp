#include "definitions/barrier_between_kernels.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<BarrierBetweenKernels> registerTestCase{};

class BarrierBetweenKernelsTest : public ::testing::TestWithParam<std::tuple<WorkItemIdUsage, size_t, size_t, size_t>> {
};

TEST_P(BarrierBetweenKernelsTest, Test) {
    BarrierBetweenKernelsArguments args{};
    args.api = Api::L0;
    args.usedIds = std::get<0>(GetParam());
    args.measuredCommands = std::get<1>(GetParam());
    args.workgroupCount = std::get<2>(GetParam());
    args.workgroupSize = std::get<3>(GetParam());

    BarrierBetweenKernels test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    BarrierBetweenKernelsTest,
    BarrierBetweenKernelsTest,
    ::testing::Combine(
        ::testing::Values(WorkItemIdUsage::None, WorkItemIdUsage::Global, WorkItemIdUsage::Local),
        ::testing::Values(500),
        ::CommonGtestArgs::workgroupCount(),
        ::CommonGtestArgs::workgroupSize()));
