#include "api_overhead_benchmark/definitions/execute_command_list.h"

#include <gtest/gtest.h>

class ExecuteCommandListTest : public ::testing::TestWithParam<bool> {
};

TEST_P(ExecuteCommandListTest, Test) {
    ExecuteCommandListArguments args{};
    args.api = Api::L0;
    args.useFence = GetParam();

    ExecuteCommandList test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    ExecuteCommandListTest,
    ExecuteCommandListTest,
    ::testing::Values(false, true));
