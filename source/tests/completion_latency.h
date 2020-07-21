#pragma once

#include "framework/levelzero.h"
#include "framework/test_case.h"
#include "framework/timer.h"

#include <gtest/gtest.h>
#include <level_zero/zex_ddi.h>

namespace UllsTest {

struct CompletionLatencyArguments : TestCaseArguments {};

class CompletionLatency : public TestCase<CompletionLatencyArguments> {
  public:
    using TestCase<CompletionLatencyArguments>::TestCase;

    std::string getHelp() override {
        return "enqueues system memory write and measures time between the moment when update is visible on CPU and"
               "the moment when synchronizing call returns\n";
    }

    std::string getTestCaseName() override {
        return "CompletionLatency";
    }

    bool runL0(const CompletionLatencyArguments &arguments, Statistics &statistics) override {
        LevelZero levelzero;
        zex_pfnCommandListAppendPipeControl_t zexCommandListAppendPipeControl{};
        constexpr static auto bufferSize = 4096u;
        const auto extensionLoaded = zeDriverGetExtensionFunctionAddress(levelzero.driver, "zexCommandListAppendPipeControl", (void **)&zexCommandListAppendPipeControl);
        if (extensionLoaded != ZE_RESULT_SUCCESS) {
            // Cannot run this benchmark without this experimental API
            return false;
        }
        Timer timer;

        ze_host_mem_alloc_desc_t allocationDesc{ZE_HOST_MEM_ALLOC_DESC_VERSION_CURRENT, ZE_HOST_MEM_ALLOC_FLAG_DEFAULT};
        void *buffer = nullptr;
        ASSERT_ZE_RESULT_SUCCESS(zeDriverAllocHostMem(levelzero.driver, &allocationDesc, bufferSize, 0, &buffer));
        ASSERT_ZE_RESULT_SUCCESS(zeDeviceMakeMemoryResident(levelzero.device, buffer, bufferSize));

        // Create two command lists, one writes 0x0 to the memory location, the other one writes 0x1
        const ze_command_list_desc_t cmdListDesc = {ZE_COMMAND_LIST_DESC_VERSION_CURRENT};
        ze_command_list_handle_t cmdLists[2];
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.device, &cmdListDesc, &cmdLists[0]));
        ASSERT_ZE_RESULT_SUCCESS(zexCommandListAppendPipeControl(cmdLists[0], buffer, 0x0));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdLists[0]));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.device, &cmdListDesc, &cmdLists[1]));
        ASSERT_ZE_RESULT_SUCCESS(zexCommandListAppendPipeControl(cmdLists[1], buffer, 0x1));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdLists[1]));

        // Warmup
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdLists[1], nullptr));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdLists[0], nullptr));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));

        // Alternate between writing 0x0 and writing 0x1 and measure time
        int currentValueToWrite = 1;
        for (auto i = 0; i < arguments.iterations; i++) {
            ze_command_list_handle_t currentCommandList = cmdLists[currentValueToWrite];

            ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &currentCommandList, nullptr));
            while (*((uint64_t *)buffer) != currentValueToWrite) {
            }
            timer.measureStart();
            ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));
            timer.messureEnd();
            statistics.pushValue(timer.Get());

            currentValueToWrite ^= 1;
        }

        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));
        ASSERT_ZE_RESULT_SUCCESS(zeDeviceEvictMemory(levelzero.device, buffer, bufferSize));

        ASSERT_ZE_RESULT_SUCCESS(zeDriverFreeMem(levelzero.driver, buffer));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdLists[0]));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdLists[1]));
        return true;
    }
};

} // namespace UllsTest
