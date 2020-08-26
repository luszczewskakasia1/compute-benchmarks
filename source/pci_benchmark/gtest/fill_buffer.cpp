#include "pci_benchmark/definitions/fill_buffer.h"

#include <gtest/gtest.h>

class FillBufferTest : public ::testing::TestWithParam<std::tuple<Api, size_t, size_t, bool>> {
};

TEST_P(FillBufferTest, Test) {
    FillBufferArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    args.patternSize = std::get<2>(GetParam());
    args.compressed = std::get<3>(GetParam());

    FillBuffer test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    FillBufferTest,
    FillBufferTest,
    ::testing::Combine(
        ::testing::Values(Api::OpenCL, Api::L0),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(16, 128),
        ::testing::Values(false, true)));
