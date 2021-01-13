#include "definitions/usm_copy.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"
static const inline RegisterTestCase<UsmCopy> registerTestCase{};

#include <gtest/gtest.h>

class UsmCopyTest : public ::testing::TestWithParam<std::tuple<Api, MemoryPlacement, MemoryPlacement, size_t, BufferContents, bool, bool>> {
};

TEST_P(UsmCopyTest, Test) {
    UsmCopyArguments args;
    args.api = std::get<0>(GetParam());
    args.sourcePlacement = std::get<1>(GetParam());
    args.destinationPlacement = std::get<2>(GetParam());
    args.size = std::get<3>(GetParam());
    args.contents = std::get<4>(GetParam());
    args.forceBlitter = std::get<5>(GetParam());
    args.useEvents = std::get<6>(GetParam());

    UsmCopy test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    UsmCopyTest,
    UsmCopyTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(MemoryPlacement::Device, MemoryPlacement::Host, MemoryPlacement::Shared),
        ::testing::Values(MemoryPlacement::Device, MemoryPlacement::Host, MemoryPlacement::Shared),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(BufferContents::Zeros),
        ::testing::Values(false, true),
        ::testing::Values(false)));
