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

#include "definitions/one_atomic_explicit.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<OneAtomicExplicit> registerTestCase{};

class OneAtomicExplicitTest : public ::testing::TestWithParam<std::tuple<DataType, MathOperation, AtomicScope, AtomicMemoryOrder, CommonGtestArgs::EnqueueSize>> {
};

TEST_P(OneAtomicExplicitTest, Test) {
    OneAtomicExplicitArguments args{};
    args.api = Api::OpenCL;
    args.dataType = std::get<0>(GetParam());
    args.atomicOperation = std::get<1>(GetParam());
    args.scope = std::get<2>(GetParam());
    args.memoryOrder = std::get<3>(GetParam());
    args.workgroupCount = std::get<4>(GetParam()).workgroupCount;
    args.workgroupSize = std::get<4>(GetParam()).workgroupSize;

    OneAtomicExplicit test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    OneAtomicExplicitTest,
    OneAtomicExplicitTest,
    ::testing::Combine(
        ::testing::Values(DataType::Float, DataType::Int32),
        ::CommonGtestArgs::allAtomicMathOperations(),
        ::testing::ValuesIn(AtomicScopeHelper::allValues),
        ::testing::ValuesIn(AtomicMemoryOrderHelper::allValues),
        ::CommonGtestArgs::enqueueSizesForAtomics()));
