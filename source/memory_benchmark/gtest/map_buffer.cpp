#include "memory_benchmark/definitions/map_buffer.h"

#include "framework/common_gtest_args.h"

#include <gtest/gtest.h>

class MapBufferTest : public ::testing::TestWithParam<std::tuple<Api, size_t, bool, MapFlags>> {
};

TEST_P(MapBufferTest, Test) {
    MapBufferArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    args.compressed = std::get<2>(GetParam());
    args.mapFlags = std::get<3>(GetParam());

    MapBuffer test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    MapBufferTest,
    MapBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true),
        ::testing::Values(MapFlags::Read, MapFlags::Write, MapFlags::WriteInvalidate)));
