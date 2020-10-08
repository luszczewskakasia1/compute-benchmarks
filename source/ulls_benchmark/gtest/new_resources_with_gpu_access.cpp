#include "ulls_benchmark/definitions/new_resources_with_gpu_access.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<NewResourcesWithGpuAccess> registerTestCase{};

class NewResourcesWithGpuAccessTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(NewResourcesWithGpuAccessTest, Test) {
    NewResourcesWithGpuAccessArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());

    NewResourcesWithGpuAccess test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = kiloByte * kiloByte;
constexpr size_t gigaByte = 1024u * megaByte;

INSTANTIATE_TEST_SUITE_P(
    NewResourcesWithGpuAccessTest,
    NewResourcesWithGpuAccessTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(kiloByte, 64 * kiloByte, 512 * kiloByte, 1 * megaByte, 16 * megaByte, 64 * megaByte, 256 * megaByte, 1 * gigaByte)));
