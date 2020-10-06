#include "framework/l0/levelzero.h"
#include "framework/l0/memory_placement_helper_l0.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/random_helper.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/usm_fill_specific_pattern.h"
#include "memory_benchmark/timer_helper.h"

#include <gtest/gtest.h>

static TestResult run(const UsmFillSpecificPatternArguments &arguments, Statistics &statistics) {
    LevelZero levelzero(false);
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueCreate(levelzero.context, levelzero.device, levelzero.commandQueueDescCopy.get(), &levelzero.commandQueue));
    Timer timer;

    // Create buffer
    void *buffer{};
    ASSERT_ZE_RESULT_SUCCESS(zeMemAllocHostOrDeviceOrShared(arguments.memoryPlacement, levelzero.context, levelzero.device, arguments.bufferSize, &buffer));

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

    // Create command list
    ze_command_list_desc_t cmdListDesc{};
    cmdListDesc.commandQueueGroupOrdinal = levelzero.commandQueueDescCopy->ordinal;
    const std::vector<uint8_t> &pattern = arguments.pattern;
    ze_command_list_handle_t cmdList{};
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.context, levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendMemoryFill(cmdList, buffer, pattern.data(), pattern.size(), arguments.bufferSize, event, 0, nullptr));
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
            TimerHelper::pushValue(statistics, commandTime, arguments.bufferSize);
        } else {
            TimerHelper::pushValueFromTimer(statistics, timer, arguments.bufferSize);
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

static RegisterTestCaseImplementation<UsmFillSpecificPattern> registerTestCase(run, Api::L0);
