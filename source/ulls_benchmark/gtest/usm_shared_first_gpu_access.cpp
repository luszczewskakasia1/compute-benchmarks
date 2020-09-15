#include "ulls_benchmark/definitions/usm_shared_first_gpu_access.h"

#include "framework/common_gtest_args.h"

#include <gtest/gtest.h>

class UsmSharedFirstGpuAccessTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(UsmSharedFirstGpuAccessTest, Test) {
    UsmSharedFirstGpuAccessArguments args;
    args.api = std::get<0>(GetParam());
    args.bufferSize = std::get<1>(GetParam());

    UsmSharedFirstGpuAccess test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;

INSTANTIATE_TEST_SUITE_P(
    UsmSharedFirstGpuAccessTest,
    UsmSharedFirstGpuAccessTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(64 * megaByte, 128 * megaByte)));
