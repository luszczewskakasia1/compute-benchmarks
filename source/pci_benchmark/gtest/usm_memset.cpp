#include "pci_benchmark/usm_memset.h"

#include <gtest/gtest.h>

class UsmMemsetTest : public ::testing::TestWithParam<std::tuple<Api, MemoryPlacement, size_t>> {
};

TEST_P(UsmMemsetTest, Test) {
    UsmMemsetArguments args;
    args.api = std::get<0>(GetParam());
    args.memoryPlacement = std::get<1>(GetParam());
    args.bufferSize = std::get<2>(GetParam());

    UsmMemset test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    UsmMemsetTest,
    UsmMemsetTest,
    ::testing::Combine(
        ::testing::Values(Api::OpenCL, Api::L0),
        ::testing::Values(MemoryPlacement::Host, MemoryPlacement::Device),
        ::testing::Values(128 * megaByte, 512 * megaByte)));
