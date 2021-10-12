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

#include "definitions/kernel_and_copy.h"

#include "framework/test_case/register_test_case.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<KernelAndCopy> registerTestCase{};

class KernelAndCopyTest : public ::testing::TestWithParam<std::tuple<bool, bool, bool, bool>> {
};

TEST_P(KernelAndCopyTest, Test) {
    KernelAndCopyArguments args{};
    args.api = Api::OpenCL;
    args.twoQueues = std::get<0>(GetParam());
    args.runKernel = std::get<1>(GetParam());
    args.runCopy = std::get<2>(GetParam());
    args.useCopyQueue = std::get<3>(GetParam());

    KernelAndCopy test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    KernelAndCopyTest,
    KernelAndCopyTest,
    ::testing::Values(
        // Without copy queue
        std::make_tuple(false, true, false, false), // only kernel
        std::make_tuple(false, false, true, false), // only copy
        std::make_tuple(false, true, true, false),  // kernel+copy on same queue
        std::make_tuple(true, true, true, false),   // kernel+copy on different queues

        // With copy queue
        std::make_tuple(false, false, true, true), // only copy
        std::make_tuple(true, true, true, true)    // kernel+copy on different queues
        ));
