#include "multitile_memory_benchmark/definitions/fill_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<FillBuffer> registerTestCase{};

class FillBufferTest : public ::testing::TestWithParam<std::tuple<DeviceSelection, DeviceSelection, DeviceSelection, size_t, size_t, bool, bool>> {};

TEST_P(FillBufferTest, Test) {
    FillBufferArguments args;
    args.api = Api::OpenCL;
    args.contextPlacement = std::get<0>(GetParam());
    args.queuePlacement = std::get<1>(GetParam());
    args.bufferPlacement = std::get<2>(GetParam());
    args.size = std::get<3>(GetParam());
    args.patternSize = std::get<4>(GetParam());
    args.compressed = std::get<5>(GetParam());
    args.copyQueue = std::get<6>(GetParam());

    if (!DeviceSelectionHelper::isSubset(args.contextPlacement, args.queuePlacement) ||
        !DeviceSelectionHelper::isSubset(args.contextPlacement, args.bufferPlacement)) {
        GTEST_SKIP(); // Placement of all resources must be a subset of context placement
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
        ::CommonGtestArgs::contextDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(1, 16, 128),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
