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

#include "definitions/destroy_command_list_immediate.h"

#include "framework/test_case/register_test_case.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<DestroyCommandListImmediate> registerTestCase{};

class DestroyCommandListImmediateTest : public ::testing::TestWithParam<size_t> {
};

TEST_P(DestroyCommandListImmediateTest, Test) {
    DestroyCommandListImmediateArguments args{};
    args.api = Api::L0;
    args.cmdListCount = GetParam();
    DestroyCommandListImmediate test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    DestroyCommandListImmediateTest,
    DestroyCommandListImmediateTest,
    ::testing::Values(1, 10, 100, 1000));
