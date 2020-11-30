#include "atomic_benchmark/definitions/one_atomic.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<OneAtomic> registerTestCase{};

class OneAtomicTest : public ::testing::TestWithParam<std::tuple<DataType, AtomicOperation, CommonGtestArgs::EnqueueSize>> {
};

TEST_P(OneAtomicTest, Test) {
    OneAtomicArguments args{};
    args.api = Api::OpenCL;
    args.dataType = std::get<0>(GetParam());
    args.atomicOperation = std::get<1>(GetParam());
    args.workgroupCount = std::get<2>(GetParam()).workgroupCount;
    args.workgroupSize = std::get<2>(GetParam()).workgroupSize;

    OneAtomic test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    OneAtomicTest,
    OneAtomicTest,
    ::testing::Combine(
        ::testing::Values(DataType::Float, DataType::Int32),
        ::CommonGtestArgs::allAtomicOperations(),
        ::CommonGtestArgs::enqueueSizesForAtomics()));
