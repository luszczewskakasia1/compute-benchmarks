#include "memory_benchmark/definitions/unmap_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<UnmapBuffer> registerTestCase{};

class UnmapBufferTest : public ::testing::TestWithParam<std::tuple<Api, size_t, bool, MapFlags>> {
};

TEST_P(UnmapBufferTest, Test) {
    UnmapBufferArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    args.compressed = std::get<2>(GetParam());
    args.mapFlags = std::get<3>(GetParam());

    UnmapBuffer test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    UnmapBufferTest,
    UnmapBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true),
        ::testing::Values(MapFlags::Read, MapFlags::Write, MapFlags::WriteInvalidate)));
