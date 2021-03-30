#include "definitions/simultaneous_blits.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<SimultaneousBlits> registerTestCase{};

class SimultaneousBlitterCopiesTest : public ::testing::TestWithParam<std::tuple<Api, UsmMemoryPlacement, UsmMemoryPlacement, size_t, std::bitset<9>>> {
};

TEST_P(SimultaneousBlitterCopiesTest, Test) {
    SimultaneousBlitterCopiesArguments args;
    args.api = std::get<0>(GetParam());
    args.sourcePlacement = std::get<1>(GetParam());
    args.destinationPlacement = std::get<2>(GetParam());
    args.size = std::get<3>(GetParam());
    args.blitters = std::get<4>(GetParam());

    SimultaneousBlits test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    SimultaneousBlitterCopiesTest,
    SimultaneousBlitterCopiesTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(UsmMemoryPlacement::Device),
        ::testing::Values(UsmMemoryPlacement::Device),
        ::testing::Values(512 * megaByte),
        ::testing::Values("000000111", "111111111")));
