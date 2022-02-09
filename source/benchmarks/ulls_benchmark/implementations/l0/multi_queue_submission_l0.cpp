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
#include "framework/utility/file_helper.h"
#include "framework/utility/timer.h"

#include "definitions/multi_queue_submission.h"

#include <gtest/gtest.h>

static TestResult run(const MultiQueueSubmissionArguments &arguments, Statistics &statistics) {
    // Setup
    QueueProperties queueProperties = QueueProperties::create().disable();
    LevelZero levelzero(queueProperties);
    Timer timer;

    std::vector<ze_command_queue_handle_t> queues;
    std::vector<void *> buffers;

    // Create kernel
    auto spirvModule = FileHelper::loadBinaryFile("ulls_benchmark_write_one_global_ids.spv");
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
    kernelDesc.pKernelName = "write_one";
    ASSERT_ZE_RESULT_SUCCESS(zeKernelCreate(module, &kernelDesc, &kernel));

    uint32_t groupSizeX = static_cast<uint32_t>(arguments.workgroupSize);
    uint32_t groupSizeY = 1u;
    uint32_t groupSizeZ = 1u;
    ASSERT_ZE_RESULT_SUCCESS(zeKernelSetGroupSize(kernel, groupSizeX, groupSizeY, groupSizeZ));

    ze_group_count_t dispatchTraits;
    dispatchTraits.groupCountX = static_cast<uint32_t>(arguments.workgroupCount);
    dispatchTraits.groupCountY = 1u;
    dispatchTraits.groupCountZ = 1u;

    // Create command queues and buffers
    size_t gws = arguments.workgroupCount * arguments.workgroupSize;
    size_t size = gws * sizeof(int);
    ze_command_queue_desc_t commandQueueDesc{ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC};
    for (size_t i = 0; i < arguments.queueCount; i++) {
        queues.push_back(levelzero.createQueue(levelzero.device, commandQueueDesc));

        const ze_device_mem_alloc_desc_t deviceAllocDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
        void *pBuffer = nullptr;
        ASSERT_ZE_RESULT_SUCCESS(zeMemAllocDevice(levelzero.context, &deviceAllocDesc, size, 0, levelzero.device, &pBuffer));
        buffers.push_back(pBuffer);
    }

    // Warmup
    ze_command_list_desc_t cmdListDesc{};
    cmdListDesc.commandQueueGroupOrdinal = levelzero.commandQueueDesc.ordinal;
    ze_command_list_handle_t cmdList;
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.context, levelzero.device, &cmdListDesc, &cmdList));
    for (size_t i = 0; i < arguments.queueCount; i++) {
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListReset(cmdList));
        ASSERT_ZE_RESULT_SUCCESS(zeKernelSetArgumentValue(kernel, 0, sizeof(void *), &buffers[i]));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendLaunchKernel(cmdList, kernel, &dispatchTraits, nullptr, 0, nullptr));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdList));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(queues[i], 1, &cmdList, nullptr));
    }
    for (size_t i = 0; i < arguments.queueCount; i++) {
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(queues[i], std::numeric_limits<uint64_t>::max()));
    }

    // Benchmark
    for (auto i = 0u; i < arguments.iterations; i++) {
        timer.measureStart();
        for (size_t i = 0; i < arguments.queueCount; i++) {
            ASSERT_ZE_RESULT_SUCCESS(zeCommandListReset(cmdList));
            ASSERT_ZE_RESULT_SUCCESS(zeKernelSetArgumentValue(kernel, 0, sizeof(void *), &buffers[i]))
            ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendLaunchKernel(cmdList, kernel, &dispatchTraits, nullptr, 0, nullptr));
            ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdList));
            ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(queues[i], 1, &cmdList, nullptr));
        }
        for (size_t i = 0; i < arguments.queueCount; i++) {
            ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(queues[i], std::numeric_limits<uint64_t>::max()));
        }
        timer.measureEnd();
        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);
    }

    ASSERT_ZE_RESULT_SUCCESS(zeKernelDestroy(kernel));
    ASSERT_ZE_RESULT_SUCCESS(zeModuleDestroy(module));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    for (size_t i = 0; i < arguments.queueCount; i++) {
        ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, buffers[i]));
    }
    return TestResult::Success;
}

static RegisterTestCaseImplementation<MultiQueueSubmission> registerTestCase(run, Api::L0);
