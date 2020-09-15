#include "memory_benchmark/definitions/usm_shared_migrate_gpu.h"

#include "framework/common_gtest_args.h"

#include <gtest/gtest.h>

class UsmSharedMigrateGpuTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(UsmSharedMigrateGpuTest, Test) {
    UsmSharedMigrateGpuArguments args;
    args.api = std::get<0>(GetParam());
    args.bufferSize = std::get<1>(GetParam());

    UsmSharedMigrateGpu test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    UsmSharedMigrateGpuTest,
    UsmSharedMigrateGpuTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(128 * megaByte, 256 * megaByte)));
