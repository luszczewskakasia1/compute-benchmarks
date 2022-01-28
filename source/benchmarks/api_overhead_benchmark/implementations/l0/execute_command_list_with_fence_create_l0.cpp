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

#include "framework/l0/levelzero.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/buffer_contents_helper.h"
#include "framework/utility/file_helper.h"
#include "framework/utility/timer.h"

#include "definitions/execute_command_list_with_fence_create.h"

#include <gtest/gtest.h>

static TestResult run(const ExecuteCommandListWithFenceCreateArguments &arguments, Statistics &statistics) {
    // Setup
    LevelZero levelzero;
    Timer timer;

    // Create command list
    ze_command_list_desc_t cmdListDesc{};
    ze_command_list_handle_t cmdList;
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.context, levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdList));

    const ze_fence_desc_t fenceDesc{ZE_STRUCTURE_TYPE_FENCE_DESC, nullptr, 0};
    ze_fence_handle_t fence = nullptr;
    // Warmup
    ASSERT_ZE_RESULT_SUCCESS(zeFenceCreate(levelzero.commandQueue, &fenceDesc, &fence));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, fence));
    ASSERT_ZE_RESULT_SUCCESS(zeFenceHostSynchronize(fence, std::numeric_limits<uint64_t>::max()));
    ASSERT_ZE_RESULT_SUCCESS(zeFenceDestroy(fence));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint64_t>::max()));

    // Benchmark
    for (auto i = 0u; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_ZE_RESULT_SUCCESS(zeFenceCreate(levelzero.commandQueue, &fenceDesc, &fence));
        timer.measureEnd();
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, fence));
        ASSERT_ZE_RESULT_SUCCESS(zeFenceHostSynchronize(fence, std::numeric_limits<uint64_t>::max()));
        ASSERT_ZE_RESULT_SUCCESS(zeFenceDestroy(fence));

        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);

        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint64_t>::max()));
    }

    // Cleanup
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<ExecuteCommandListWithFenceCreate> registerTestCase(run, Api::L0);
