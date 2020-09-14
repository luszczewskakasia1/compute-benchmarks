#include "framework/l0/levelzero.h"
#include "framework/load_binary_file.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "gpu_cmds_benchmark/definitions/wait_on_event_cold.h"

#include <gtest/gtest.h>
#include <level_zero/zex_ddi.h>

struct TestResources {
    TestResources(LevelZero &levelzero, size_t eventsCount, uint64_t *beginTimestamp, uint64_t *endTimestamp)
        : events(eventsCount) {
        // Create events and signal them
        const ze_event_pool_desc_t eventPoolDesc = {ZE_STRUCTURE_TYPE_EVENT_POOL_DESC, nullptr, 0, static_cast<uint32_t>(eventsCount)};
        const ze_event_desc_t eventDesc = {ZE_STRUCTURE_TYPE_EVENT_DESC, nullptr, 0, ZE_EVENT_SCOPE_FLAG_HOST, ZE_EVENT_SCOPE_FLAG_DEVICE};
        ZE_RESULT_SUCCESS_OR_TERMINATE(zeEventPoolCreate(levelzero.context, &eventPoolDesc, 0, nullptr, &this->eventPool));
        for (size_t eventIndex = 0u; eventIndex < eventsCount; eventIndex++) {
            auto &event = this->events[eventIndex];
            ZE_RESULT_SUCCESS_OR_TERMINATE(zeEventCreate(this->eventPool, &eventDesc, &event));
            ZE_RESULT_SUCCESS_OR_TERMINATE(zeEventHostSignal(event));
        }

        // Create command list
        ze_command_list_desc_t cmdListDesc{};
        cmdListDesc.commandQueueGroupOrdinal = levelzero.commandListCommandQueueGroupOrdinal;
        ZE_RESULT_SUCCESS_OR_TERMINATE(zeCommandListCreate(levelzero.context, levelzero.device, &cmdListDesc, &this->cmdList));
        ZE_RESULT_SUCCESS_OR_TERMINATE(zeCommandListAppendWriteGlobalTimestamp(this->cmdList, beginTimestamp, nullptr, 0, nullptr));
        for (size_t commandIndex = 0u; commandIndex < eventsCount; commandIndex++) {
            ZE_RESULT_SUCCESS_OR_TERMINATE(zeCommandListAppendWaitOnEvents(this->cmdList, 1, &this->events[commandIndex]));
        }
        ZE_RESULT_SUCCESS_OR_TERMINATE(zeCommandListAppendWriteGlobalTimestamp(this->cmdList, endTimestamp, nullptr, 0, nullptr));
        ZE_RESULT_SUCCESS_OR_TERMINATE(zeCommandListClose(this->cmdList));
    }

    ~TestResources() {
        EXPECT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
        for (auto &event : events) {
            EXPECT_ZE_RESULT_SUCCESS(zeEventDestroy(event));
        }
        EXPECT_ZE_RESULT_SUCCESS(zeEventPoolDestroy(eventPool));
    }

    ze_event_pool_handle_t eventPool{};
    std::vector<ze_event_handle_t> events{};
    ze_command_list_handle_t cmdList{};
};

static TestResult run(const WaitOnEventColdArguments &arguments, Statistics &statistics) {
    LevelZero levelzero;

    // Create buffer
    const ze_device_mem_alloc_desc_t deviceAllocationDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
    const ze_host_mem_alloc_desc_t hostAllocationDesc{ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC};
    void *buffer = nullptr;
    const auto bufferSize = sizeof(uint64_t) * 2;
    ASSERT_ZE_RESULT_SUCCESS(zeMemAllocShared(levelzero.context, &deviceAllocationDesc, &hostAllocationDesc, bufferSize, 0, levelzero.device, &buffer));
    ASSERT_ZE_RESULT_SUCCESS(zeContextMakeMemoryResident(levelzero.context, levelzero.device, buffer, bufferSize))
    uint64_t *beginTimestamp = static_cast<uint64_t *>(buffer);
    uint64_t *endTimestamp = beginTimestamp + 1;

    // Warmup
    auto testResources = std::make_unique<TestResources>(levelzero, arguments.measuredCommands, beginTimestamp, endTimestamp);
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &testResources->cmdList, nullptr));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint64_t>::max()));
    testResources.reset();

    // Benchmark
    for (auto i = 0; i < arguments.iterations; i++) {
        testResources = std::make_unique<TestResources>(levelzero, arguments.measuredCommands, beginTimestamp, endTimestamp);
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &testResources->cmdList, nullptr));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint64_t>::max()));
        testResources.reset();

        auto commandTime = static_cast<Statistics::Value>(*endTimestamp - *beginTimestamp);
        commandTime *= levelzero.deviceProperties.timerResolution;
        commandTime /= arguments.measuredCommands;
        commandTime /= 1000;
        statistics.pushValue(commandTime);
    }

    ASSERT_ZE_RESULT_SUCCESS(zeContextEvictMemory(levelzero.context, levelzero.device, buffer, bufferSize));
    ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<WaitOnEventCold> registerTestCase(run, Api::L0);
