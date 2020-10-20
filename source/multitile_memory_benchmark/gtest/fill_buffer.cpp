#include "multitile_memory_benchmark/definitions/fill_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<FillBuffer> registerTestCase{};

class FillBufferTest : public ::testing::TestWithParam<std::tuple<DeviceSelection, DeviceSelection, Api, size_t, size_t, bool, bool>> {};

TEST_P(FillBufferTest, Test) {
    FillBufferArguments args;
    args.queuePlacement = std::get<0>(GetParam());
    args.bufferPlacement = std::get<1>(GetParam());
    args.api = std::get<2>(GetParam());
    args.size = std::get<3>(GetParam());
    args.patternSize = std::get<4>(GetParam());
    args.compressed = std::get<5>(GetParam());
    args.copyQueue = std::get<6>(GetParam());

    if (args.queuePlacement == DeviceSelection::Root && args.bufferPlacement == DeviceSelection::Root) {
        GTEST_SKIP(); // nothing ninja here
    }

    FillBuffer test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    FillBufferTest,
    FillBufferTest,
    ::testing::Combine(
        ::testing::Values(DeviceSelection::Root, DeviceSelection::Tile0, DeviceSelection::Tile1),
        ::testing::Values(DeviceSelection::Root, DeviceSelection::Tile0, DeviceSelection::Tile1),
        ::CommonGtestArgs::allApis(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(1, 16, 128),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
