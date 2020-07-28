#include "tests/new_resources_submission_host.h"

#include <gtest/gtest.h>

class NewResourcesSubmissionHostTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(NewResourcesSubmissionHostTest, Test) {
    UllsTest::NewResourcesSubmissionHostArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());

    UllsTest::NewResourcesSubmissionHost test;
    test.run(args);
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = kiloByte * kiloByte;
constexpr size_t gigaByte = 1024u * megaByte;

INSTANTIATE_TEST_SUITE_P(
    NewResourcesSubmissionHostTest,
    NewResourcesSubmissionHostTest,
    ::testing::Combine(
        ::testing::Values(Api::OpenCL, Api::L0),
        ::testing::Values(1, 64 * kiloByte, 512 * kiloByte, 1 * megaByte, 16 * megaByte, 64 * megaByte, 256 * megaByte, 1 * gigaByte)));
