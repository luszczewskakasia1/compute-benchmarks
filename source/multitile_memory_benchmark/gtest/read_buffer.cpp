#include "multitile_memory_benchmark/definitions/read_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

static const inline RegisterTestCase<ReadBuffer> registerTestCase{};

class ReadBufferTest : public ::testing::TestWithParam<std::tuple<DeviceSelection, DeviceSelection, DeviceSelection, size_t, bool, bool>> {};

TEST_P(ReadBufferTest, Test) {
    ReadBufferArguments args;
    args.api = Api::OpenCL;
    args.contextPlacement = std::get<0>(GetParam());
    args.queuePlacement = std::get<1>(GetParam());
    args.bufferPlacement = std::get<2>(GetParam());
    args.size = std::get<3>(GetParam());
    args.compressed = std::get<4>(GetParam());
    args.useEvents = std::get<5>(GetParam());

    if (!DeviceSelectionHelper::isSubset(args.contextPlacement, args.queuePlacement) ||
        !DeviceSelectionHelper::isSubset(args.contextPlacement, args.bufferPlacement)) {
        GTEST_SKIP(); // Placement of all resources must be a subset of context placement
    }

    ReadBuffer test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    ReadBufferTest,
    ReadBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::contextDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::CommonGtestArgs::resourceDeviceSelections(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
