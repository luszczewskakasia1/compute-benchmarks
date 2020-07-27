#include "framework/levelzero.h"
#include "framework/load_binary_file.h"
#include "framework/register_test_case.h"
#include "framework/timer.h"
#include "tests/new_resources_submission_host.h"

#include <gtest/gtest.h>
#include <level_zero/zex_ddi.h>

namespace UllsTest {
static TestResult run(const NewResourcesSubmissionHostArguments &arguments, Statistics &statistics) {
    LevelZero levelzero;
    Timer timer;

    // Create kernel
    auto spirvModule = loadBinaryFile("write_one.spv");
    if (spirvModule.size() == 0) {
        return TestResult::KernelNotFound;
    }
    ze_module_handle_t module;
    ze_kernel_handle_t kernel;
    ze_module_desc_t moduleDesc = {ZE_MODULE_DESC_VERSION_CURRENT};
    moduleDesc.format = ZE_MODULE_FORMAT_IL_SPIRV;
    moduleDesc.pInputModule = reinterpret_cast<const uint8_t *>(spirvModule.data());
    moduleDesc.inputSize = spirvModule.size();
    EXPECT_ZE_RESULT_SUCCESS(zeModuleCreate(levelzero.device, &moduleDesc, &module, nullptr));
    ze_kernel_desc_t kernelDesc = {ZE_KERNEL_DESC_VERSION_CURRENT};
    kernelDesc.pKernelName = "write_one";
    EXPECT_ZE_RESULT_SUCCESS(zeKernelCreate(module, &kernelDesc, &kernel));

    // Configure kernel
    uint32_t groupSizeX = 1u;
    uint32_t groupSizeY = 1u;
    uint32_t groupSizeZ = 1u;
    EXPECT_ZE_RESULT_SUCCESS(zeKernelSetGroupSize(kernel, groupSizeX, groupSizeY, groupSizeZ));

    // Create buffer for warmup
    const auto bufferSize = arguments.size;
    ze_host_mem_alloc_desc_t allocationDesc{ZE_HOST_MEM_ALLOC_DESC_VERSION_CURRENT, ZE_HOST_MEM_ALLOC_FLAG_DEFAULT};
    uint64_t *buffer = nullptr;
    ASSERT_ZE_RESULT_SUCCESS(zeDriverAllocHostMem(levelzero.driver, &allocationDesc, bufferSize, 0, (void **)(&buffer)));
    ASSERT_ZE_RESULT_SUCCESS(zeDeviceMakeMemoryResident(levelzero.device, buffer, bufferSize));

    // Warmup
    const ze_command_list_desc_t cmdListDesc = {ZE_COMMAND_LIST_DESC_VERSION_CURRENT};
    const ze_group_count_t dispatchTraits{1, 1, 1};
    ze_command_list_handle_t cmdList;
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeKernelSetArgumentValue(kernel, 0, sizeof(buffer), &buffer));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendLaunchKernel(cmdList, kernel, &dispatchTraits, nullptr, 0, nullptr));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));

    // Cleanup after warmup
    ASSERT_ZE_RESULT_SUCCESS(zeDeviceEvictMemory(levelzero.device, buffer, bufferSize));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeDriverFreeMem(levelzero.driver, buffer));

    // Benchmark
    for (auto i = 0; i < arguments.iterations; i++) {
        timer.measureStart();

        // Create buffer
        ASSERT_ZE_RESULT_SUCCESS(zeDriverAllocHostMem(levelzero.driver, &allocationDesc, bufferSize, 0, (void **)(&buffer)));
        ASSERT_ZE_RESULT_SUCCESS(zeDeviceMakeMemoryResident(levelzero.device, buffer, bufferSize));

        // Create command list to write 1
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.device, &cmdListDesc, &cmdList));
        ASSERT_ZE_RESULT_SUCCESS(zeKernelSetArgumentValue(kernel, 0, sizeof(buffer), &buffer));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendLaunchKernel(cmdList, kernel, &dispatchTraits, nullptr, 0, nullptr));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListClose(cmdList));

        // Dispatch kernel and wait for completion
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));

        timer.measureEnd();

        // Cleanup after iteration
        ASSERT_ZE_RESULT_SUCCESS(zeDeviceEvictMemory(levelzero.device, buffer, bufferSize));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
        ASSERT_ZE_RESULT_SUCCESS(zeDriverFreeMem(levelzero.driver, buffer));

        statistics.pushValue(timer.Get());
    }

    ASSERT_ZE_RESULT_SUCCESS(zeKernelDestroy(kernel));
    ASSERT_ZE_RESULT_SUCCESS(zeModuleDestroy(module));
    return TestResult::Success;
}

static RegisterTestCase<NewResourcesSubmissionHost> registerTestCase(run, Api::L0);
} // namespace UllsTest
