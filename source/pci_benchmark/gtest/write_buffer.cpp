#include "pci_benchmark/write_buffer.h"

#include <gtest/gtest.h>

class WriteBufferTest : public ::testing::TestWithParam<std::tuple<Api, size_t, bool>> {
};

TEST_P(WriteBufferTest, Test) {
    WriteBufferArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    args.compressed = std::get<2>(GetParam());

    WriteBuffer test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    WriteBufferTest,
    WriteBufferTest,
    ::testing::Combine(
        ::testing::Values(Api::OpenCL, Api::L0),
        ::testing::Values(128 * megaByte, 512 * megaByte),
        ::testing::Values(false, true)));
