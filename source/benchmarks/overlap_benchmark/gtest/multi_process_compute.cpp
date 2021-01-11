#include "definitions/multi_process_compute.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<MultiProcessCompute> registerTestCase{};

class MultiProcessComputeTest : public ::testing::TestWithParam<std::tuple<Api, size_t, DeviceSelection>> {
};

TEST_P(MultiProcessComputeTest, Test) {
    MultiProcessComputeArguments args{};
    args.api = std::get<0>(GetParam());
    args.bufferSize = std::get<1>(GetParam());
    args.deviceSelection = std::get<2>(GetParam());

    MultiProcessCompute test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    MultiProcessComputeTest,
    MultiProcessComputeTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(1024, 2048),
        ::testing::Values(DeviceSelection::Tile0, DeviceSelection::Tile1, DeviceSelection::Tile0 | DeviceSelection::Tile1)));

