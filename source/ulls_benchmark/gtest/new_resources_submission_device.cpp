#include "ulls_benchmark/definitions/new_resources_submission_device.h"

#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

class NewResourcesSubmissionDeviceTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(NewResourcesSubmissionDeviceTest, Test) {
    NewResourcesSubmissionDeviceArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());

    NewResourcesSubmissionDevice test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = kiloByte * kiloByte;
constexpr size_t gigaByte = 1024u * megaByte;

INSTANTIATE_TEST_SUITE_P(
    NewResourcesSubmissionDeviceTest,
    NewResourcesSubmissionDeviceTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(1, 64 * kiloByte, 512 * kiloByte, 1 * megaByte, 16 * megaByte, 64 * megaByte, 256 * megaByte, 1 * gigaByte)));
