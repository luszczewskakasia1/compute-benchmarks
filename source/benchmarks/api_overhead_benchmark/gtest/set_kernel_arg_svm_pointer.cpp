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

#include "definitions/set_kernel_arg_svm_pointer.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<SetKernelArgSvmPointer> registerTestCase{};

class SetKernelArgSvmPointerTest : public ::testing::TestWithParam<std::tuple<Api, size_t, bool, size_t>> {
};

TEST_P(SetKernelArgSvmPointerTest, Test) {
    SetKernelArgSvmPointerArguments args{};
    args.api = std::get<0>(GetParam());
    args.allocationsCount = std::get<1>(GetParam());
    args.reallocate = std::get<2>(GetParam());
    args.allocationSize = std::get<3>(GetParam());

    SetKernelArgSvmPointer test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    SetKernelArgSvmPointerTest,
    SetKernelArgSvmPointerTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(100),
        ::testing::Values(false, true),
        ::testing::Values(1024)));
