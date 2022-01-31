/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#include "definitions/execute_command_list_with_indirect.h"

#include "framework/test_case/register_test_case.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<ExecuteCommandListWithInidrect> registerTestCase{};

class ExecuteCommandListTestWithIndirectTest : public ::testing::TestWithParam<std::tuple<size_t, UsmMemoryPlacement>> {
};

TEST_P(ExecuteCommandListTestWithIndirectTest, Test) {
    ExecuteCommandListWithIndirectArguments args{};
    args.api = Api::L0;
    args.IndirectAllocationsAmount = std::get<0>(GetParam());
    args.placement = std::get<1>(GetParam());

    ExecuteCommandListWithInidrect test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    ExecuteCommandListTestWithIndirectTest,
    ExecuteCommandListTestWithIndirectTest,
    ::testing::Combine(
        ::testing::Values(0u, 10u, 100u, 1000u, 10000u),
        ::testing::Values(UsmMemoryPlacement::Shared, UsmMemoryPlacement::Host)));
