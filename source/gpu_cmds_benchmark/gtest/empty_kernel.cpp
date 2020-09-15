#include "gpu_cmds_benchmark/definitions/empty_kernel.h"

#include "framework/common_gtest_args.h"

#include <gtest/gtest.h>

class EmptyKernelTest : public ::testing::TestWithParam<std::tuple<size_t, size_t, size_t>> {
};

TEST_P(EmptyKernelTest, Test) {
    EmptyKernelArguments args{};
    args.api = Api::L0;
    args.measuredCommands = std::get<0>(GetParam());
    args.workgroupCount = std::get<1>(GetParam());
    args.workgroupSize = std::get<2>(GetParam());

    EmptyKernel test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    EmptyKernelTest,
    EmptyKernelTest,
    ::testing::Combine(
        ::testing::Values(500),
        ::CommonGtestArgs::workgroupCount(),
        ::CommonGtestArgs::workgroupSize()));
