#include "atomic_benchmark/definitions/separate_atomic.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<SeparateAtomics> registerTestCase{};

class SeparateAtomicsTest : public ::testing::TestWithParam<std::tuple<DataType, AtomicOperation, size_t, size_t, size_t>> {
};

TEST_P(SeparateAtomicsTest, Test) {
    SeparateAtomicsArguments args{};
    args.api = Api::OpenCL;
    args.dataType = std::get<0>(GetParam());
    args.atomicOperation = std::get<1>(GetParam());
    args.atomicsPerCacheline = std::get<2>(GetParam());
    args.workgroupCount = std::get<3>(GetParam());
    args.workgroupSize = std::get<4>(GetParam());

    SeparateAtomics test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    SeparateAtomicsTest,
    SeparateAtomicsTest,
    ::testing::Combine(
        ::testing::Values(DataType::Float, DataType::Int32),
        ::CommonGtestArgs::allAtomicOperations(),
        ::testing::Values(1, 4),
        ::testing::Values(1u, 1000u),
        ::testing::Values(16u, 256u)));
