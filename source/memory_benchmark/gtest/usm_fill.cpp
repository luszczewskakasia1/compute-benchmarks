#include "memory_benchmark/definitions/usm_fill.h"

#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

class UsmFillTest : public ::testing::TestWithParam<std::tuple<Api, MemoryPlacement, size_t, size_t, bool, bool, BufferContents>> {
};

TEST_P(UsmFillTest, Test) {
    UsmFillArguments args;
    args.api = std::get<0>(GetParam());
    args.memoryPlacement = std::get<1>(GetParam());
    args.bufferSize = std::get<2>(GetParam());
    args.patternSize = std::get<3>(GetParam());
    args.copyQueue = std::get<4>(GetParam());
    args.useEvents = std::get<5>(GetParam());
    args.patternContents = std::get<6>(GetParam());

    UsmFill test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    UsmFillTest,
    UsmFillTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(MemoryPlacement::Host, MemoryPlacement::Device, MemoryPlacement::Shared),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(1, 16, 256),
        ::testing::Values(false, true),
        ::testing::Values(false, true),
        ::testing::Values(BufferContents::Zeros, BufferContents::Random)));
