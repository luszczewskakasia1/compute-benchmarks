#include "tests/ulls_submission_with_empty_kernel.h"

class UllsSubmissionWithEmptyKernelTest : public ::testing::TestWithParam<std::tuple<Api, size_t, size_t>> {
};

TEST_P(UllsSubmissionWithEmptyKernelTest, Test) {
    UllsSubmissionWithEmptyKernelArguments args;
    args.workgroupCount = std::get<1>(GetParam());
    args.workgroupSize = std::get<2>(GetParam());

    UllsSubmissionWithEmptyKernel test{std::get<Api>(GetParam()), args, 10};
    test.run();
}

INSTANTIATE_TEST_SUITE_P(
    UllsSubmissionWithEmptyKernelTest,
    UllsSubmissionWithEmptyKernelTest,
    ::testing::Combine(
        ::testing::Values(Api::OpenCL, Api::L0),
        ::testing::Values(1, 100, 1000, 10000),
        ::testing::Values(1, 16, 32, 256)));
