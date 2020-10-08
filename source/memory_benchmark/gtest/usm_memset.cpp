#include "memory_benchmark/definitions/usm_memset.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<UsmMemset> registerTestCase{};

class UsmMemsetTest : public ::testing::TestWithParam<std::tuple<Api, MemoryPlacement, size_t, bool>> {
};

TEST_P(UsmMemsetTest, Test) {
    UsmMemsetArguments args;
    args.api = std::get<0>(GetParam());
    args.memoryPlacement = std::get<1>(GetParam());
    args.bufferSize = std::get<2>(GetParam());
    args.copyQueue = std::get<3>(GetParam());

    UsmMemset test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    UsmMemsetTest,
    UsmMemsetTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(MemoryPlacement::Host, MemoryPlacement::Device, MemoryPlacement::Shared),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true)));
