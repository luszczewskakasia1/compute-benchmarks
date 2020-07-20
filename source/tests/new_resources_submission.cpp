#include "tests/new_resources_submission.h"

class NewResourcesSubmissionTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(NewResourcesSubmissionTest, Test) {
    UllsTest::NewResourcesSubmissionArguments args;
    args.iterations = 10;
    args.sizeMB = std::get<1>(GetParam());

    UllsTest::NewResourcesSubmission test{std::get<Api>(GetParam()), args};
    test.run();
}

INSTANTIATE_TEST_SUITE_P(
    NewResourcesSubmissionTest,
    NewResourcesSubmissionTest,
    ::testing::Combine(
        ::testing::Values(Api::OpenCL, Api::L0),
        ::testing::Values(1, 16, 64, 256)));
