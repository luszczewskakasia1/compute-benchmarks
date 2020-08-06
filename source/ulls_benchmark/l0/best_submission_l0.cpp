#include "framework/l0/levelzero.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "ulls_benchmark/best_submission.h"

#include <emmintrin.h>
#include <gtest/gtest.h>
#include <level_zero/zex_ddi.h>

static TestResult run(const BestSubmissionArguments &arguments, Statistics &statistics) {
    LevelZero levelzero;
    zex_pfnCommandListAppendPipeControl_t zexCommandListAppendPipeControl{};
    constexpr static auto bufferSize = 4096u;
    // const auto extensionLoaded = zeDriverGetExtensionFunctionAddress(levelzero.driver, "zexCommandListAppendPipeControl", (void **)&zexCommandListAppendPipeControl);
    const auto extensionLoaded = ZE_RESULT_ERROR_UNKNOWN;
    if (extensionLoaded != ZE_RESULT_SUCCESS) {
        return TestResult::DriverFunctionNotFound;
    }
    Timer timer;

    // Create buffer
    const ze_host_mem_alloc_desc_t allocationDesc{ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC};
    void *buffer = nullptr;
    ASSERT_ZE_RESULT_SUCCESS(zeMemAllocHost(levelzero.context, &allocationDesc, bufferSize, 0, &buffer));
    ASSERT_ZE_RESULT_SUCCESS(zeContextMakeMemoryResident(levelzero.context, levelzero.device, buffer, bufferSize));
    volatile uint64_t *volatileBuffer = static_cast<uint64_t *>(buffer);

    // Create command list writing 1 to the buffer
    const ze_command_list_desc_t cmdListDesc{ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC};
    ze_command_list_handle_t cmdList;
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.context, levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zexCommandListAppendPipeControl(cmdList, buffer, 1));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdList));

    // Warmup
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));

    // Benchmark
    for (auto i = 0; i < arguments.iterations; i++) {
        *volatileBuffer = 0;
        _mm_clflush(buffer);

        timer.measureStart();
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
        while (*volatileBuffer != 1) {
        }
        timer.measureEnd();
        statistics.pushValue(timer.Get());

        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));
    }

    ASSERT_ZE_RESULT_SUCCESS(zeContextEvictMemory(levelzero.context, levelzero.device, buffer, bufferSize));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, buffer));
    return TestResult::Success;
}

static RegisterTestCase<BestSubmission> registerTestCase(run, Api::L0);
