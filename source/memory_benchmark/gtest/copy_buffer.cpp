#include "memory_benchmark/definitions/copy_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<CopyBuffer> registerTestCase{};

class CopyBufferTest : public ::testing::TestWithParam<std::tuple<DeviceSelection, Api, size_t, bool, bool, bool>> {
};

TEST_P(CopyBufferTest, Test) {
    CopyBufferArguments args;
    args.device = std::get<0>(GetParam());
    args.api = std::get<1>(GetParam());
    args.size = std::get<2>(GetParam());
    args.compressedSource = std::get<3>(GetParam());
    args.compressedDestination = std::get<4>(GetParam());
    args.useEvents = std::get<5>(GetParam());

    CopyBuffer test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    CopyBufferTest,
    CopyBufferTest,
    ::testing::Combine(
        ::testing::Values(DeviceSelection::Root, DeviceSelection::Tile0, DeviceSelection::Tile2),
        ::CommonGtestArgs::allApis(),
        ::testing::Values(1, 128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
