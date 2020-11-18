#include "ulls_benchmark/definitions/usm_shared_first_cpu_access.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<UsmSharedFirstCpuAccess> registerTestCase{};

class UsmSharedFirstCpuAccessTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(UsmSharedFirstCpuAccessTest, Test) {
    UsmSharedFirstCpuAccessArguments args;
    args.api = std::get<0>(GetParam());
    args.bufferSize = std::get<1>(GetParam());

    UsmSharedFirstCpuAccess test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    UsmSharedFirstCpuAccessTest,
    UsmSharedFirstCpuAccessTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(64 * megaByte, 128 * megaByte)));
