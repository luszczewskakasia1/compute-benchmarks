#include "framework/l0/levelzero.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/usm_fill.h"

#include <gtest/gtest.h>

static TestResult run(const UsmFillArguments &arguments, Statistics &statistics) {
    LevelZero levelzero(false);
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueCreate(levelzero.context, levelzero.device, levelzero.commandQueueDescCopy.get(), &levelzero.commandQueue));
    Timer timer;

    // Create buffers
    const ze_host_mem_alloc_desc_t hostAllocationDesc{ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC};
    const ze_device_mem_alloc_desc_t deviceAllocationDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
    void *buffer{};
    if (isDeviceMemory(arguments.memoryPlacement)) {
        ASSERT_ZE_RESULT_SUCCESS(zeMemAllocDevice(levelzero.context, &deviceAllocationDesc, arguments.bufferSize, 0, levelzero.device, &buffer));
    } else {
        ASSERT_ZE_RESULT_SUCCESS(zeMemAllocHost(levelzero.context, &hostAllocationDesc, arguments.bufferSize, 0, &buffer));
    }

    // Create event
    ze_event_pool_handle_t eventPool{};
    ze_event_handle_t event{};
    if (arguments.useEvents) {
        ze_event_pool_desc_t eventPoolDesc{ZE_STRUCTURE_TYPE_EVENT_POOL_DESC};
        eventPoolDesc.flags = ZE_EVENT_POOL_FLAG_KERNEL_TIMESTAMP;
        eventPoolDesc.count = 1;
        ASSERT_ZE_RESULT_SUCCESS(zeEventPoolCreate(levelzero.context, &eventPoolDesc, 1, &levelzero.device, &eventPool));
        ze_event_desc_t eventDesc{ZE_STRUCTURE_TYPE_EVENT_DESC};
        eventDesc.index = 0;
        eventDesc.signal = ZE_EVENT_SCOPE_FLAG_DEVICE;
        eventDesc.wait = ZE_EVENT_SCOPE_FLAG_HOST;
        ASSERT_ZE_RESULT_SUCCESS(zeEventCreate(eventPool, &eventDesc, &event));
    }

    // Make the buffer  resident
    ASSERT_ZE_RESULT_SUCCESS(zeContextMakeMemoryResident(levelzero.context, levelzero.device, buffer, arguments.bufferSize));

    // Create pattern
    const auto pattern = std::make_unique<uint8_t[]>(arguments.patternSize);

    // Create command list
    ze_command_list_desc_t cmdListDesc{};
    cmdListDesc.commandQueueGroupOrdinal = levelzero.commandQueueDescCopy->ordinal;
    ze_command_list_handle_t cmdList{};
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.context, levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendMemoryFill(cmdList, buffer, pattern.get(), arguments.patternSize, arguments.bufferSize, event, 0, nullptr));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdList));

    // Warmup
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));

    // Benchmark
    for (auto i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));
        timer.measureEnd();

        if (arguments.useEvents) {
            ze_kernel_timestamp_result_t timestampResult{};
            ASSERT_ZE_RESULT_SUCCESS(zeEventQueryKernelTimestamp(event, &timestampResult));
            auto commandTime = static_cast<Statistics::Value>(timestampResult.global.kernelEnd - timestampResult.global.kernelStart);
            commandTime *= levelzero.deviceProperties.timerResolution;
            statistics.pushValue(Timer::getBandwidth(commandTime, arguments.bufferSize));
        } else {
            statistics.pushValue(timer.getBandwidth(arguments.bufferSize));
        }
    }

    // Evict buffers
    ASSERT_ZE_RESULT_SUCCESS(zeContextEvictMemory(levelzero.context, levelzero.device, buffer, arguments.bufferSize));

    // Cleanup
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    if (arguments.useEvents) {
        ASSERT_ZE_RESULT_SUCCESS(zeEventPoolDestroy(eventPool));
        ASSERT_ZE_RESULT_SUCCESS(zeEventDestroy(event));
    }
    ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmFill> registerTestCase(run, Api::L0);
