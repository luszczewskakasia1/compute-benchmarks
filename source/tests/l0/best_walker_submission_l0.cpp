#include "framework/levelzero.h"
#include "framework/load_binary_file.h"
#include "framework/register_test_case.h"
#include "framework/timer.h"
#include "tests/best_walker_submission.h"

#include <emmintrin.h>
#include <gtest/gtest.h>
#include <level_zero/zex_ddi.h>

namespace UllsTest {
static TestResult run(const BestWalkerSubmissionArguments &arguments, Statistics &statistics) {
    LevelZero levelzero;
    constexpr static auto bufferSize = 4096u;
    Timer timer;

    // Create buffer
    ze_host_mem_alloc_desc_t allocationDesc{ZE_HOST_MEM_ALLOC_DESC_VERSION_CURRENT, ZE_HOST_MEM_ALLOC_FLAG_DEFAULT};
    void *buffer = nullptr;
    ASSERT_ZE_RESULT_SUCCESS(zeDriverAllocHostMem(levelzero.driver, &allocationDesc, bufferSize, 0, &buffer));
    ASSERT_ZE_RESULT_SUCCESS(zeDeviceMakeMemoryResident(levelzero.device, buffer, bufferSize));
    volatile uint64_t *volatileBuffer = static_cast<uint64_t *>(buffer);

    // Create kernel
    const auto kernelBinary = loadBinaryFile("write_one.spv");
    if (kernelBinary.size() == 0) {
        return TestResult::KernelNotFound;
    }
    ze_module_desc_t moduleDesc{};
    moduleDesc.version = ZE_MODULE_DESC_VERSION_CURRENT;
    moduleDesc.format = ZE_MODULE_FORMAT_IL_SPIRV;
    moduleDesc.inputSize = kernelBinary.size();
    moduleDesc.pInputModule = kernelBinary.data();
    moduleDesc.pBuildFlags = nullptr;
    moduleDesc.pConstants = nullptr;
    ze_module_handle_t module;
    ASSERT_ZE_RESULT_SUCCESS(zeModuleCreate(levelzero.device, &moduleDesc, &module, nullptr));
    ze_kernel_desc_t kernelDesc{};
    kernelDesc.version = ZE_KERNEL_DESC_VERSION_CURRENT;
    kernelDesc.flags = ZE_KERNEL_FLAG_NONE;
    kernelDesc.pKernelName = "write_one";
    ze_kernel_handle_t kernel;
    ASSERT_ZE_RESULT_SUCCESS(zeKernelCreate(module, &kernelDesc, &kernel));

    // Configure kernel
    ASSERT_ZE_RESULT_SUCCESS(zeKernelSetGroupSize(kernel, 1, 1, 1));
    ASSERT_ZE_RESULT_SUCCESS(zeKernelSetArgumentValue(kernel, 0, sizeof(buffer), &buffer));

    // Create command lists writing 0 or 1 to the buffer
    const ze_group_count_t groupCount{1, 1, 1};
    const ze_command_list_desc_t cmdListDesc = {ZE_COMMAND_LIST_DESC_VERSION_CURRENT};
    ze_command_list_handle_t cmdList;
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendLaunchKernel(cmdList, kernel, &groupCount, nullptr, 0, nullptr));
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

        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));
        statistics.pushValue(timer.Get());
    }

    ASSERT_ZE_RESULT_SUCCESS(zeDeviceEvictMemory(levelzero.device, buffer, bufferSize));
    ASSERT_ZE_RESULT_SUCCESS(zeKernelDestroy(kernel));
    ASSERT_ZE_RESULT_SUCCESS(zeModuleDestroy(module));
    ASSERT_ZE_RESULT_SUCCESS(zeDriverFreeMem(levelzero.driver, buffer));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    return TestResult::Success;
}

static RegisterTestCase<BestWalkerSubmission> registerTestCase(run, Api::L0);
} // namespace UllsTest
