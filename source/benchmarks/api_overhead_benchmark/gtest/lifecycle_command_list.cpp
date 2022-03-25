/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2022 Intel Corporation
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

#include "definitions/lifecycle_command_list.h"

#include "framework/test_case/register_test_case.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<LifecycleCommandList> registerTestCase{};

class LifecycleCommandListTest : public ::testing::TestWithParam<std::tuple<size_t, bool>> {
};

TEST_P(LifecycleCommandListTest, Test) {
    LifecycleCommandListArguments args{};
    args.api = Api::L0;
    args.cmdListCount = std::get<0>(GetParam());
    args.copyOnly = std::get<1>(GetParam());
    LifecycleCommandList test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    LifecycleCommandListTest,
    LifecycleCommandListTest,
    ::testing::Combine(
        ::testing::Values(1, 10, 100),
        ::testing::Values(false, true)));
