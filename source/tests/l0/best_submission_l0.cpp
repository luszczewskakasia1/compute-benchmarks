#include "framework/levelzero.h"
#include "framework/register_test_case.h"
#include "framework/timer.h"
#include "tests/best_submission.h"

#include <emmintrin.h>
#include <gtest/gtest.h>
#include <level_zero/zex_ddi.h>

namespace UllsTest {
static bool run(const BestSubmissionArguments &arguments, Statistics &statistics) {
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
    uint64_t *buffer = nullptr;
    ASSERT_ZE_RESULT_SUCCESS(zeDriverAllocHostMem(levelzero.driver, &allocationDesc, bufferSize, 0, (void **)(&buffer)));
    ASSERT_ZE_RESULT_SUCCESS(zeDeviceMakeMemoryResident(levelzero.device, buffer, bufferSize));

    // Create command list writing 1 to the buffer
    const ze_command_list_desc_t cmdListDesc = {ZE_COMMAND_LIST_DESC_VERSION_CURRENT};
    ze_command_list_handle_t cmdList;
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zexCommandListAppendPipeControl(cmdList, buffer, 1));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdList));

    // Warmup
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));

    // Benchmark
    int currentValueToWrite = 1;
    for (auto i = 0; i < arguments.iterations; i++) {
        *buffer = 0;
        _mm_clflush(buffer);

        timer.measureStart();
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
        while (*buffer != 1) {
        }
        timer.measureEnd();
        statistics.pushValue(timer.Get());

        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));
    }

    ASSERT_ZE_RESULT_SUCCESS(zeDeviceEvictMemory(levelzero.device, buffer, bufferSize));
    ASSERT_ZE_RESULT_SUCCESS(zeDriverFreeMem(levelzero.driver, buffer));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    return true;
}

static RegisterTestCase<BestSubmission> registerTestCase(run, Api::L0);
} // namespace UllsTest
