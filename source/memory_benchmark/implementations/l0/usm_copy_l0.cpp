#include "framework/l0/levelzero.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "memory_benchmark/definitions/usm_copy.h"

#include <gtest/gtest.h>

static TestResult run(const UsmCopyArguments &arguments, Statistics &statistics) {
    LevelZero levelzero;
    Timer timer;

    // Create buffers
    const ze_host_mem_alloc_desc_t hostAllocationDesc{ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC};
    const ze_device_mem_alloc_desc_t deviceAllocationDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
    void *source{}, *destination{};
    if (isDeviceMemory(arguments.transferDirection, TransferOperand::Source)) {
        ASSERT_ZE_RESULT_SUCCESS(zeMemAllocDevice(levelzero.context, &deviceAllocationDesc, arguments.size, 0, levelzero.device, &source));
    } else {
        ASSERT_ZE_RESULT_SUCCESS(zeMemAllocHost(levelzero.context, &hostAllocationDesc, arguments.size, 0, &source));
    }
    if (isDeviceMemory(arguments.transferDirection, TransferOperand::Destination)) {
        ASSERT_ZE_RESULT_SUCCESS(zeMemAllocDevice(levelzero.context, &deviceAllocationDesc, arguments.size, 0, levelzero.device, &destination));
    } else {
        ASSERT_ZE_RESULT_SUCCESS(zeMemAllocHost(levelzero.context, &hostAllocationDesc, arguments.size, 0, &destination));
    }

    // Make buffers resident
    ASSERT_ZE_RESULT_SUCCESS(zeContextMakeMemoryResident(levelzero.context, levelzero.device, source, arguments.size));
    ASSERT_ZE_RESULT_SUCCESS(zeContextMakeMemoryResident(levelzero.context, levelzero.device, destination, arguments.size));

    // Create command list
    ze_command_list_desc_t cmdListDesc{};
    cmdListDesc.commandQueueGroupOrdinal = levelzero.commandListCommandQueueGroupOrdinal;
    ze_command_list_handle_t cmdList{};
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.context, levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendMemoryCopy(cmdList, destination, source, arguments.size, nullptr, 0, nullptr));
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
        statistics.pushValue(timer.getBandwidth(arguments.size));
    }

    // Evict buffers
    ASSERT_ZE_RESULT_SUCCESS(zeContextEvictMemory(levelzero.context, levelzero.device, destination, arguments.size));
    ASSERT_ZE_RESULT_SUCCESS(zeContextEvictMemory(levelzero.context, levelzero.device, source, arguments.size));

    // Cleanup
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, destination));
    ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, source));
    return TestResult::Success;
}

static RegisterTestCase<UsmCopy> registerTestCase(run, Api::L0);
