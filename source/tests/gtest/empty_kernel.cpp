#include "tests/empty_kernel.h"

#include <gtest/gtest.h>

class EmptyKernelSubmissionTest : public ::testing::TestWithParam<std::tuple<Api, size_t, size_t>> {
};

TEST_P(EmptyKernelSubmissionTest, Test) {
    UllsTest::EmptyKernelArguments args;
    args.api = std::get<0>(GetParam());
    args.workgroupCount = std::get<1>(GetParam());
    args.workgroupSize = std::get<2>(GetParam());

    UllsTest::EmptyKernel test{args};
    test.run();
}

INSTANTIATE_TEST_SUITE_P(
    EmptyKernelSubmissionTest,
    EmptyKernelSubmissionTest,
    ::testing::Combine(
        ::testing::Values(Api::OpenCL, Api::L0),
        ::testing::Values(1, 100, 1000, 10000),
        ::testing::Values(1, 16, 32, 256)));
