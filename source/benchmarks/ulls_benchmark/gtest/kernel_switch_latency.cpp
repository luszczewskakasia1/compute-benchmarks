#include "definitions/kernel_switch_latency.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<KernelSwitchLatency> registerTestCase{};

class KernelSwitchLatencyTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(KernelSwitchLatencyTest, Test) {
    KernelSwitchLatencyArguments args;
    args.api = std::get<0>(GetParam());
    args.kernelCount = std::get<1>(GetParam());

    KernelSwitchLatency test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    KernelSwitchLatencyTest,
    KernelSwitchLatencyTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(16, 32, 64, 128)));
