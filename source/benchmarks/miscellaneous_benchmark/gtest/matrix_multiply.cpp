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

#include "definitions/matrix_multiply.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<MatrixMultiply> registerTestCase{};

class MatrixMultiplyTest : public ::testing::TestWithParam<std::tuple<Api, size_t, size_t, size_t>> {
};

TEST_P(MatrixMultiplyTest, Test) {
    MatrixMultiplyArguments args;
    args.api = std::get<0>(GetParam());
    args.numberOfElementsX = std::get<1>(GetParam());
    args.numberOfElementsY = std::get<2>(GetParam());
    args.numberOfElementsZ = std::get<3>(GetParam());

    MatrixMultiply test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    MatrixMultiplyTest,
    MatrixMultiplyTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(128, 256, 512),
        ::testing::Values(128, 256, 512, 513),
        ::testing::Values(128, 256, 512, 513)));
