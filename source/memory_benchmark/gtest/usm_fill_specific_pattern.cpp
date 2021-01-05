#include "memory_benchmark/definitions/usm_fill_specific_pattern.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<UsmFillSpecificPattern> registerTestCase{};

class UsmFillSpecificPatternTest : public ::testing::TestWithParam<std::tuple<Api, MemoryPlacement, size_t, BufferContents, std::string, bool, bool>> {
};

TEST_P(UsmFillSpecificPatternTest, Test) {
    UsmFillSpecificPatternArguments args;
    args.api = std::get<0>(GetParam());
    args.memoryPlacement = std::get<1>(GetParam());
    args.bufferSize = std::get<2>(GetParam());
    args.contents = std::get<3>(GetParam());
    args.pattern = std::get<4>(GetParam());
    args.forceBlitter = std::get<5>(GetParam());
    args.useEvents = std::get<6>(GetParam());

    UsmFillSpecificPattern test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    UsmFillSpecificPatternTest,
    UsmFillSpecificPatternTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(MemoryPlacement::Host, MemoryPlacement::Device, MemoryPlacement::Shared),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(BufferContents::Zeros),
        ::testing::Values("0x01AA0B"),
        ::testing::Values(false, true),
        ::testing::Values(false)));
