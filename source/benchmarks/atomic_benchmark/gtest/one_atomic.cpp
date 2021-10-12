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

#include "definitions/one_atomic.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<OneAtomic> registerTestCase{};

class OneAtomicTest : public ::testing::TestWithParam<std::tuple<DataType, MathOperation, CommonGtestArgs::EnqueueSize>> {
};

TEST_P(OneAtomicTest, Test) {
    OneAtomicArguments args{};
    args.api = Api::OpenCL;
    args.dataType = std::get<0>(GetParam());
    args.atomicOperation = std::get<1>(GetParam());
    args.workgroupCount = std::get<2>(GetParam()).workgroupCount;
    args.workgroupSize = std::get<2>(GetParam()).workgroupSize;

    OneAtomic test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    OneAtomicTest,
    OneAtomicTest,
    ::testing::Combine(
        ::testing::Values(DataType::Float, DataType::Int32),
        ::CommonGtestArgs::allAtomicMathOperations(),
        ::CommonGtestArgs::enqueueSizesForAtomics()));
