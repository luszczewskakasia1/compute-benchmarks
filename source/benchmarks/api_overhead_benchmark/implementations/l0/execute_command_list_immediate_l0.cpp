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

#include "framework/l0/levelzero.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/file_helper.h"
#include "framework/utility/timer.h"

#include "definitions/execute_command_list_immediate.h"

#include <gtest/gtest.h>

static TestResult run(const ExecuteCommandListImmediateArguments &arguments, Statistics &statistics) {
    // Setup
    LevelZero levelzero;
    Timer timer;

    // Create kernel
    auto spirvModule = FileHelper::loadBinaryFile("api_overhead_benchmark_empty_kernel.spv");
    if (spirvModule.size() == 0) {
        return TestResult::KernelNotFound;
    }
    ze_module_handle_t module;
    ze_kernel_handle_t kernel;
    ze_module_desc_t moduleDesc{ZE_STRUCTURE_TYPE_MODULE_DESC};
    moduleDesc.format = ZE_MODULE_FORMAT_IL_SPIRV;
    moduleDesc.pInputModule = reinterpret_cast<const uint8_t *>(spirvModule.data());
    moduleDesc.inputSize = spirvModule.size();
    ASSERT_ZE_RESULT_SUCCESS(zeModuleCreate(levelzero.context, levelzero.device, &moduleDesc, &module, nullptr));
    ze_kernel_desc_t kernelDesc{ZE_STRUCTURE_TYPE_KERNEL_DESC};
    kernelDesc.pKernelName = "empty";
    ASSERT_ZE_RESULT_SUCCESS(zeKernelCreate(module, &kernelDesc, &kernel));

    // Create event
    ze_event_pool_desc_t eventPoolDesc{ZE_STRUCTURE_TYPE_EVENT_POOL_DESC};
    eventPoolDesc.flags = ZE_EVENT_POOL_FLAG_HOST_VISIBLE;
    if (arguments.useProfiling) {
        eventPoolDesc.flags |= ZE_EVENT_POOL_FLAG_KERNEL_TIMESTAMP;
    }

    eventPoolDesc.count = 1;
    ze_event_pool_handle_t eventPool;
    ASSERT_ZE_RESULT_SUCCESS(zeEventPoolCreate(levelzero.context, &eventPoolDesc, 1, &levelzero.device, &eventPool));
    ze_event_desc_t eventDesc{ZE_STRUCTURE_TYPE_EVENT_DESC};
    eventDesc.index = 0;
    eventDesc.signal = ZE_EVENT_SCOPE_FLAG_DEVICE;
    eventDesc.wait = ZE_EVENT_SCOPE_FLAG_HOST;
    ze_event_handle_t event{};
    ASSERT_ZE_RESULT_SUCCESS(zeEventCreate(eventPool, &eventDesc, &event));

    // Configure kernel
    ASSERT_ZE_RESULT_SUCCESS(zeKernelSetGroupSize(kernel, 1u, 1u, 1u));

    // Create an immediate command list
    const ze_group_count_t groupCount{1, 1, 1};
    ze_command_queue_desc_t commandQueueDesc{ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC};
    commandQueueDesc.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
    ze_command_list_handle_t cmdList;
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreateImmediate(levelzero.context, levelzero.device, &commandQueueDesc, &cmdList));

    // Warmup
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendLaunchKernel(cmdList, kernel, &groupCount, event, 0, nullptr));
    ASSERT_ZE_RESULT_SUCCESS(zeEventHostSynchronize(event, std::numeric_limits<uint64_t>::max()));
    ASSERT_ZE_RESULT_SUCCESS(zeEventHostReset(event));

    // Benchmark
    for (auto i = 0u; i < arguments.iterations; i++) {
        timer.measureStart();
        for (uint32_t callId = 0u; callId < arguments.amountOfCalls - 1; callId++) {
            ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendLaunchKernel(cmdList, kernel, &groupCount, event, 0, nullptr));
        }
        //last call synchronizes
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendLaunchKernel(cmdList, kernel, &groupCount, event, 0, nullptr));
        ASSERT_ZE_RESULT_SUCCESS(zeEventHostSynchronize(event, std::numeric_limits<uint64_t>::max()));
        timer.measureEnd();
        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);
    }
    ASSERT_ZE_RESULT_SUCCESS(zeEventDestroy(event));
    ASSERT_ZE_RESULT_SUCCESS(zeEventPoolDestroy(eventPool));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeKernelDestroy(kernel));
    ASSERT_ZE_RESULT_SUCCESS(zeModuleDestroy(module));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<ExecuteCommandListImmediate> registerTestCase(run, Api::L0);
