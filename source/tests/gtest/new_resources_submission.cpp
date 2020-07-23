#include "tests/new_resources_submission.h"

#include <gtest/gtest.h>

class NewResourcesSubmissionTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(NewResourcesSubmissionTest, Test) {
    UllsTest::NewResourcesSubmissionArguments args;
    args.api = std::get<0>(GetParam());
    args.iterations = ::gtestIterations;
    args.size = std::get<1>(GetParam());

    UllsTest::NewResourcesSubmission test{args};
    test.run();
}

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = kiloByte * kiloByte;
constexpr size_t gigaByte = 1024u * megaByte;

INSTANTIATE_TEST_SUITE_P(
    NewResourcesSubmissionTest,
    NewResourcesSubmissionTest,
    ::testing::Combine(
        ::testing::Values(Api::OpenCL, Api::L0),
        ::testing::Values(1, 64 * kiloByte, 512 * kiloByte, 1 * megaByte, 16 * megaByte, 64 * megaByte, 256 * megaByte, 1 * gigaByte)));
