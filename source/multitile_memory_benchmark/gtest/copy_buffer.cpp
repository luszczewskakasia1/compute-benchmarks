#include "multitile_memory_benchmark/definitions/copy_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

static const inline RegisterTestCase<CopyBuffer> registerTestCase{};

class CopyBufferTest : public ::testing::TestWithParam<std::tuple<DeviceSelection, DeviceSelection, DeviceSelection, DeviceSelection, size_t, bool, bool, bool>> {};

TEST_P(CopyBufferTest, Test) {
    CopyBufferArguments args;
    args.api = Api::OpenCL;
    args.contextPlacement = std::get<0>(GetParam());
    args.queuePlacement = std::get<1>(GetParam());
    args.srcPlacement = std::get<2>(GetParam());
    args.dstPlacement = std::get<3>(GetParam());
    args.size = std::get<4>(GetParam());
    args.srcCompressed = std::get<5>(GetParam());
    args.dstCompressed = std::get<6>(GetParam());
    args.useEvents = std::get<7>(GetParam());

    if (!DeviceSelectionHelper::isSubset(args.contextPlacement, args.queuePlacement) ||
        !DeviceSelectionHelper::isSubset(args.contextPlacement, args.srcPlacement) ||
        !DeviceSelectionHelper::isSubset(args.contextPlacement, args.dstPlacement)) {
        GTEST_SKIP(); // Placement of all resources must be a subset of context placement
    }

    CopyBuffer test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    CopyBufferTest,
    CopyBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::contextDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
