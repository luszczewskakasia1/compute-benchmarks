#include "memory_benchmark/definitions/usm_copy.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

static const inline RegisterTestCase<UsmCopy> registerTestCase{};

#include <gtest/gtest.h>

class UsmCopyTest : public ::testing::TestWithParam<std::tuple<Api, MemoryPlacement, MemoryPlacement, size_t, bool, bool>> {
};

TEST_P(UsmCopyTest, Test) {
    UsmCopyArguments args;
    args.api = std::get<0>(GetParam());
    args.sourcePlacement = std::get<1>(GetParam());
    args.destinationPlacement = std::get<2>(GetParam());
    args.size = std::get<3>(GetParam());
    args.copyQueue = std::get<4>(GetParam());
    args.useEvents = std::get<5>(GetParam());

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
        ::testing::Values(MemoryPlacement::Device, MemoryPlacement::Host, MemoryPlacement::Shared),
        ::testing::Values(MemoryPlacement::Device, MemoryPlacement::Host, MemoryPlacement::Shared),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
