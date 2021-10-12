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

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include "definitions/separate_atomic_explicit.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<SeparateAtomicsExplicit> registerTestCase{};

class SeparateAtomicsExplicitTest : public ::testing::TestWithParam<std::tuple<DataType, MathOperation, size_t, AtomicScope, AtomicMemoryOrder, CommonGtestArgs::EnqueueSize>> {
};

TEST_P(SeparateAtomicsExplicitTest, Test) {
    SeparateAtomicsExplicitArguments args{};
    args.api = Api::OpenCL;
    args.dataType = std::get<0>(GetParam());
    args.atomicOperation = std::get<1>(GetParam());
    args.atomicsPerCacheline = std::get<2>(GetParam());
    args.scope = std::get<3>(GetParam());
    args.memoryOrder = std::get<4>(GetParam());
    args.workgroupCount = std::get<5>(GetParam()).workgroupCount;
    args.workgroupSize = std::get<5>(GetParam()).workgroupSize;

    if (args.atomicsPerCacheline > args.workgroupCount * args.workgroupSize) {
        GTEST_SKIP();
    }

    SeparateAtomicsExplicit test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    SeparateAtomicsExplicitTest,
    SeparateAtomicsExplicitTest,
    ::testing::Combine(
        ::testing::Values(DataType::Float, DataType::Int32),
        ::CommonGtestArgs::allAtomicMathOperations(),
        ::testing::Values(1, 4),
        ::testing::ValuesIn(AtomicScopeHelper::allValues),
        ::testing::ValuesIn(AtomicMemoryOrderHelper::allValues),
        ::CommonGtestArgs::enqueueSizesForAtomics()));
