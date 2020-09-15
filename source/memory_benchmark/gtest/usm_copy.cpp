#include "memory_benchmark/definitions/usm_copy.h"

#include "framework/common_gtest_args.h"

#include <gtest/gtest.h>

class UsmCopyTest : public ::testing::TestWithParam<std::tuple<Api, TransferDirection, size_t>> {
};

TEST_P(UsmCopyTest, Test) {
    UsmCopyArguments args;
    args.api = std::get<0>(GetParam());
    args.transferDirection = std::get<1>(GetParam());
    args.size = std::get<2>(GetParam());

    UsmCopy test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    UsmCopyTest,
    UsmCopyTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(TransferDirection::SysToDev, TransferDirection::DevToSys, TransferDirection::DevToDev),
        ::testing::Values(128 * megaByte, 512 * megaByte)));
