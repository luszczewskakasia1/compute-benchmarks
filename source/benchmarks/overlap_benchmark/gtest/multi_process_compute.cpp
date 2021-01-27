#include "definitions/multi_process_compute.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<MultiProcessCompute> registerTestCase{};

class MultiProcessComputeTest : public ::testing::TestWithParam<std::tuple<Api, DeviceSelection, size_t, size_t>> {
};

TEST_P(MultiProcessComputeTest, Test) {
    MultiProcessComputeArguments args{};
    args.api = std::get<0>(GetParam());
    args.deviceSelection = std::get<1>(GetParam());
    args.bufferSize = std::get<2>(GetParam());
    args.processesPerTile = std::get<3>(GetParam());

    MultiProcessCompute test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    MultiProcessComputeTest,
    MultiProcessComputeTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(
            DeviceSelection::Tile0,
            DeviceSelection::Tile1,
            DeviceSelection::Tile0 | DeviceSelection::Tile1,
            DeviceSelection::Tile0 | DeviceSelection::Tile1 | DeviceSelection::Tile2 | DeviceSelection::Tile3),
        ::testing::Values(128 * megaByte),
        ::testing::Values(1, 2, 4)));
