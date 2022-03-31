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

#include "definitions/usm_memory_allocation.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<UsmMemoryAllocation> registerTestCase{};

class UsmMemoryAllocationTest : public ::testing::TestWithParam<std::tuple<UsmRuntimeMemoryPlacement, size_t>> {
};

TEST_P(UsmMemoryAllocationTest, Test) {
    UsmMemoryAllocationArguments args{};
    args.api = Api::L0;
    args.usmMemoryPlacement = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());
    UsmMemoryAllocation test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    UsmMemoryAllocationTest,
    UsmMemoryAllocationTest,
    ::testing::Combine(
        ::testing::Values(UsmRuntimeMemoryPlacement::Host, UsmRuntimeMemoryPlacement::Device, UsmRuntimeMemoryPlacement::Shared),
        ::testing::Values(4,
                          64,
                          512,
                          4 * MemoryConstants::kiloByte,
                          64 * MemoryConstants::kiloByte,
                          512 * MemoryConstants::kiloByte,
                          4 * MemoryConstants::megaByte,
                          64 * MemoryConstants::megaByte,
                          512 * MemoryConstants::megaByte,
                          1 * MemoryConstants::gigaByte)));
