#include "gpu_cmds_benchmark/definitions/wait_on_event_cold.h"

#include <gtest/gtest.h>

class WaitOnEventColdTest : public ::testing::TestWithParam<size_t> {
};

TEST_P(WaitOnEventColdTest, Test) {
    WaitOnEventColdArguments args{};
    args.api = Api::L0;
    args.measuredCommands = GetParam();

    WaitOnEventCold test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    WaitOnEventColdTest,
    WaitOnEventColdTest,
    ::testing::Values(500, 1000));
