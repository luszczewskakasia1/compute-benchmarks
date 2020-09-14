#include "gpu_cmds_benchmark/definitions/wait_on_event_hot.h"

#include <gtest/gtest.h>

class WaitOnEventHotTest : public ::testing::TestWithParam<size_t> {
};

TEST_P(WaitOnEventHotTest, Test) {
    WaitOnEventHotArguments args{};
    args.api = Api::L0;
    args.measuredCommands = GetParam();

    WaitOnEventHot test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    WaitOnEventHotTest,
    WaitOnEventHotTest,
    ::testing::Values(1, 10, 500));
