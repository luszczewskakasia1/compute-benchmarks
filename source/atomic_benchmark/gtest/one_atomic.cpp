#include "atomic_benchmark/definitions/one_atomic.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<OneAtomic> registerTestCase{};

class OneAtomicTest : public ::testing::TestWithParam<std::tuple<DataType, AtomicOperation, size_t, size_t>> {
};

TEST_P(OneAtomicTest, Test) {
    OneAtomicArguments args{};
    args.api = Api::OpenCL;
    args.dataType = std::get<0>(GetParam());
    args.atomicOperation = std::get<1>(GetParam());
    args.workgroupCount = std::get<2>(GetParam());
    args.workgroupSize = std::get<3>(GetParam());

    OneAtomic test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    OneAtomicTest,
    OneAtomicTest,
    ::testing::Combine(
        ::testing::Values(DataType::Float, DataType::Int32),
        ::CommonGtestArgs::allAtomicOperations(),
        ::testing::Values(1u, 1000u),
        ::testing::Values(16u, 256u)));
