#include "framework/l0/levelzero.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "pci_benchmark/usm_fill.h"

#include <gtest/gtest.h>

static TestResult run(const UsmFillArguments &arguments, Statistics &statistics) {
    LevelZero levelzero;
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

    // Make the buffer  resident
    ASSERT_ZE_RESULT_SUCCESS(zeContextMakeMemoryResident(levelzero.context, levelzero.device, buffer, arguments.bufferSize));

    // Create pattern
    const auto pattern = std::make_unique<uint8_t[]>(arguments.patternSize);

    // Create command list
    ze_command_list_desc_t cmdListDesc{};
    cmdListDesc.commandQueueGroupOrdinal = levelzero.commandListCommandQueueGroupOrdinal;
    ze_command_list_handle_t cmdList{};
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.context, levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendMemoryFill(cmdList, buffer, pattern.get(), arguments.patternSize, arguments.bufferSize, nullptr, 0, nullptr));
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
        statistics.pushValue(timer.getBandwidth(arguments.bufferSize));
    }

    // Evict buffers
    ASSERT_ZE_RESULT_SUCCESS(zeContextEvictMemory(levelzero.context, levelzero.device, buffer, arguments.bufferSize));

    // Cleanup
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, buffer));
    return TestResult::Success;
}

static RegisterTestCase<UsmFill> registerTestCase(run, Api::L0);
