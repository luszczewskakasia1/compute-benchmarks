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

#include "definitions/new_resources_with_gpu_access.h"

#include "framework/test_case/register_test_case.h"
#include "framework/utility/common_gtest_args.h"
#include "framework/utility/memory_constants.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<NewResourcesWithGpuAccess> registerTestCase{};

class NewResourcesWithGpuAccessTest : public ::testing::TestWithParam<std::tuple<Api, size_t>> {
};

TEST_P(NewResourcesWithGpuAccessTest, Test) {
    NewResourcesWithGpuAccessArguments args;
    args.api = std::get<0>(GetParam());
    args.size = std::get<1>(GetParam());

    NewResourcesWithGpuAccess test;
    test.run(args);
}

using namespace MemoryConstants;
INSTANTIATE_TEST_SUITE_P(
    NewResourcesWithGpuAccessTest,
    NewResourcesWithGpuAccessTest,
    ::testing::Combine(
        ::CommonGtestArgs::allApis(),
        ::testing::Values(kiloByte, 64 * kiloByte, 512 * kiloByte, 1 * megaByte, 16 * megaByte, 64 * megaByte, 256 * megaByte, 1 * gigaByte)));
