#include "memory_benchmark/definitions/read_buffer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<ReadBuffer> registerTestCase{};

class ReadBufferTest : public ::testing::TestWithParam<std::tuple<Api, size_t, bool, bool>> {
};

TEST_P(ReadBufferTest, Test) {
    ReadBufferArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    args.compressed = std::get<2>(GetParam());
    args.useEvents = std::get<3>(GetParam());

    ReadBuffer test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    ReadBufferTest,
    ReadBufferTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true),
        ::testing::Values(false, true)));
