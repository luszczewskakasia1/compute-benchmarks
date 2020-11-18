#include "memory_benchmark/definitions/fill_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<FillBuffer> registerTestCase{};

class FillBufferTest : public ::testing::TestWithParam<std::tuple<Api, size_t, size_t, bool, bool>> {
};

TEST_P(FillBufferTest, Test) {
    FillBufferArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    args.patternSize = std::get<2>(GetParam());
    args.compressed = std::get<3>(GetParam());
    args.copyQueue = std::get<4>(GetParam());

    FillBuffer test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    FillBufferTest,
    FillBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(1, 16, 128),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
