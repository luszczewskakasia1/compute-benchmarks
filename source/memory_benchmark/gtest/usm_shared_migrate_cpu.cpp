#include "memory_benchmark/definitions/usm_shared_migrate_cpu.h"

#include <gtest/gtest.h>

class UsmSharedMigrateCpuTest : public ::testing::TestWithParam<std::tuple<Api, bool, size_t>> {
};

TEST_P(UsmSharedMigrateCpuTest, Test) {
    UsmSharedMigrateCpuArguments args;
    args.api = std::get<0>(GetParam());
    args.accessAllBytes = std::get<1>(GetParam());
    args.bufferSize = std::get<2>(GetParam());

    UsmSharedMigrateCpu test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    UsmSharedMigrateCpuTest,
    UsmSharedMigrateCpuTest,
    ::testing::Combine(
        ::testing::Values(Api::OpenCL, Api::L0),
        ::testing::Values(false, true),
        ::testing::Values(128 * megaByte, 256 * megaByte)));
