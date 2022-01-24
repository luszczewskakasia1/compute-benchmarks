/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021-2022 Intel Corporation
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
#include "framework/l0/utility/queue_families_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/copy_submission_events.h"

#include <emmintrin.h>
#include <gtest/gtest.h>

using Clock = std::chrono::high_resolution_clock;

static TestResult run(const CopySubmissionEventsArguments &arguments, Statistics &statistics) {
    // Setup
    LevelZero levelzero;
    constexpr static auto bufferSize = 2097152u;

    auto queueFamiliesDesc = QueueFamiliesHelper::queryQueueFamilies(levelzero.device);
    bool copyEngineFound = false;
    ze_command_queue_desc_t commandQueueDesc{ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC};
    for (auto &queueDesc : queueFamiliesDesc) {
        if (queueDesc.type == EngineGroup::Copy || queueDesc.type == EngineGroup::LinkCopy) {
            copyEngineFound = true;
            commandQueueDesc.ordinal = queueDesc.ordinal;
        }
    }
    if (!copyEngineFound) {
        return TestResult::DeviceNotCapable;
    }

    const uint64_t timerResolution = levelzero.getTimerResoultion(levelzero.device);

    const ze_host_mem_alloc_desc_t allocationDesc{ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC, nullptr, ZE_HOST_MEM_ALLOC_FLAG_BIAS_UNCACHED};
    void *hostMemory = nullptr;
    ASSERT_ZE_RESULT_SUCCESS(zeMemAllocHost(levelzero.context, &allocationDesc, bufferSize, 0, &hostMemory));

    const ze_device_mem_alloc_desc_t allocationDescDevice{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC, nullptr, ZE_DEVICE_MEM_ALLOC_FLAG_BIAS_UNCACHED};
    void *destination = nullptr;
    ASSERT_ZE_RESULT_SUCCESS(zeMemAllocDevice(levelzero.context, &allocationDescDevice, bufferSize, 0, levelzero.device, &destination));

    // Create event for profiling
    const ze_event_pool_desc_t eventPoolDesc{ZE_STRUCTURE_TYPE_EVENT_POOL_DESC, nullptr, ZE_EVENT_POOL_FLAG_KERNEL_TIMESTAMP};
    uint32_t numDevices = 1;
    ze_event_pool_handle_t hEventPool;
    ASSERT_ZE_RESULT_SUCCESS(zeEventPoolCreate(levelzero.context, &eventPoolDesc, numDevices, &levelzero.device, &hEventPool));

    const ze_event_desc_t eventDesc{};
    ze_event_handle_t hEvent;
    ASSERT_ZE_RESULT_SUCCESS(zeEventCreate(hEventPool, &eventDesc, &hEvent));

    // Create immediate command list
    ze_command_list_handle_t cmdList;
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreateImmediate(levelzero.context, levelzero.device, &commandQueueDesc, &cmdList));
    uint32_t numWaitEvents = 0;
    ze_event_handle_t hWaitEvents = nullptr;

    // Warmup
    uint64_t hostEnqueueTimestamp = 0;
    uint64_t deviceEnqueueTimestamp = 0;
    ASSERT_ZE_RESULT_SUCCESS(zeDeviceGetGlobalTimestamps(levelzero.device, &hostEnqueueTimestamp, &deviceEnqueueTimestamp));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendMemoryCopy(cmdList, destination, hostMemory, bufferSize, hEvent, numWaitEvents, &hWaitEvents));
    ASSERT_ZE_RESULT_SUCCESS(zeEventHostSynchronize(hEvent, std::numeric_limits<uint64_t>::max()));

    ze_kernel_timestamp_result_t deviceStartTimestamp;
    ASSERT_ZE_RESULT_SUCCESS(zeEventQueryKernelTimestamp(hEvent, &deviceStartTimestamp));

    uint32_t truncatedDeviceEnqueueTimestamp = static_cast<uint32_t>(deviceEnqueueTimestamp);
    EXPECT_GT(deviceStartTimestamp.global.kernelStart, truncatedDeviceEnqueueTimestamp);

    // Benchmark
    for (auto i = 0u; i < arguments.iterations; i++) {
        ASSERT_ZE_RESULT_SUCCESS(zeDeviceGetGlobalTimestamps(levelzero.device, &hostEnqueueTimestamp, &deviceEnqueueTimestamp));

        ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendMemoryCopy(cmdList, destination, hostMemory, bufferSize, hEvent, numWaitEvents, &hWaitEvents));
        ASSERT_ZE_RESULT_SUCCESS(zeEventHostSynchronize(hEvent, std::numeric_limits<uint64_t>::max()));

        ASSERT_ZE_RESULT_SUCCESS(zeEventQueryKernelTimestamp(hEvent, &deviceStartTimestamp));
        truncatedDeviceEnqueueTimestamp = static_cast<uint32_t>(deviceEnqueueTimestamp);

        const auto submissionTime = std::chrono::nanoseconds((deviceStartTimestamp.global.kernelStart - truncatedDeviceEnqueueTimestamp) * timerResolution);
        statistics.pushValue(submissionTime, MeasurementUnit::Microseconds, MeasurementType::Gpu);
    }
    // Cleanup
    ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, hostMemory));
    ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, destination));

    ASSERT_ZE_RESULT_SUCCESS(zeEventDestroy(hEvent));
    ASSERT_ZE_RESULT_SUCCESS(zeEventPoolDestroy(hEventPool));

    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<CopySubmissionEvents> registerTestCase(run, Api::L0);
