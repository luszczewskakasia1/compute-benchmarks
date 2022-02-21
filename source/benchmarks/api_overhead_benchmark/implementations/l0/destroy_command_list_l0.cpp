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
#include "framework/utility/timer.h"

#include "definitions/destroy_command_list.h"

#include <gtest/gtest.h>

static TestResult run(const DestroyCommandListArguments &arguments, Statistics &statistics) {
    // Setup
    LevelZero levelzero;
    Timer timer;

    // Warmup
    std::vector<ze_command_list_handle_t> commandLists(arguments.cmdListCount);
    ze_command_list_desc_t commandListDesc = {ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC};
    commandListDesc.commandQueueGroupOrdinal = levelzero.commandQueueDesc.ordinal;
    ze_command_list_handle_t commandList;
    for (auto i = 0u; i < arguments.cmdListCount; i++) {
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.context, levelzero.device, &commandListDesc, &commandList));
        EXPECT_NE(nullptr, commandList);
        commandLists[i] = commandList;
    }
    for (auto i = 0u; i < arguments.cmdListCount; i++) {
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(commandLists[i]));
        commandLists[i] = nullptr;
    }

    // Benchmark
    for (auto j = 0u; j < arguments.iterations; j++) {

        for (auto i = 0u; i < arguments.cmdListCount; i++) {
            ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.context, levelzero.device, &commandListDesc, &commandList));
            commandLists[i] = commandList;
        }

        timer.measureStart();
        for (auto i = 0u; i < arguments.cmdListCount; i++) {
            ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(commandLists[i]));
        }
        timer.measureEnd();

        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);
    }
    return TestResult::Success;
}

static RegisterTestCaseImplementation<DestroyCommandList> registerTestCase(run, Api::L0);
