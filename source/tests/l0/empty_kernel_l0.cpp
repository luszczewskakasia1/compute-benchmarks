#include "framework/levelzero.h"
#include "framework/load_binary_file.h"
#include "framework/register_test_case.h"
#include "framework/timer.h"
#include "tests/empty_kernel.h"

#include <gtest/gtest.h>
#include <level_zero/zex_ddi.h>

namespace UllsTest {

static bool run(const EmptyKernelArguments &arguments, Statistics &statistics) {
    // Setup
    LevelZero levelzero;
    Timer timer;

    // Create kernel
    auto spirvModule = loadBinaryFile("empty_kernel.spv");
    if (spirvModule.size() == 0) {
        std::cout << " Spirv size = 0. Aborting\n";
        return false;
    }
    ze_module_handle_t module;
    ze_kernel_handle_t kernel;
    ze_module_desc_t moduleDesc = {ZE_MODULE_DESC_VERSION_CURRENT};
    moduleDesc.format = ZE_MODULE_FORMAT_IL_SPIRV;
    moduleDesc.pInputModule = reinterpret_cast<const uint8_t *>(spirvModule.data());
    moduleDesc.inputSize = spirvModule.size();
    EXPECT_ZE_RESULT_SUCCESS(zeModuleCreate(levelzero.device, &moduleDesc, &module, nullptr));
    ze_kernel_desc_t kernelDesc = {ZE_KERNEL_DESC_VERSION_CURRENT};
    kernelDesc.pKernelName = "empty";
    EXPECT_ZE_RESULT_SUCCESS(zeKernelCreate(module, &kernelDesc, &kernel));

    uint32_t groupSizeX = static_cast<uint32_t>(arguments.workgroupSize);
    uint32_t groupSizeY = 1u;
    uint32_t groupSizeZ = 1u;
    EXPECT_ZE_RESULT_SUCCESS(zeKernelSetGroupSize(kernel, groupSizeX, groupSizeY, groupSizeZ));

    ze_group_count_t dispatchTraits;
    dispatchTraits.groupCountX = static_cast<uint32_t>(arguments.workgroupCount);
    dispatchTraits.groupCountY = 1u;
    dispatchTraits.groupCountZ = 1u;

    // Create command list and append empty kernel
    const ze_command_list_desc_t cmdListDesc = {ZE_COMMAND_LIST_DESC_VERSION_CURRENT};
    ze_command_list_handle_t cmdList;
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListCreate(levelzero.device, &cmdListDesc, &cmdList));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListAppendLaunchKernel(cmdList, kernel, &dispatchTraits, nullptr, 0, nullptr));
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
        statistics.pushValue(timer.Get());
    }

    ASSERT_ZE_RESULT_SUCCESS(zeKernelDestroy(kernel));
    ASSERT_ZE_RESULT_SUCCESS(zeModuleDestroy(module));
    ASSERT_ZE_RESULT_SUCCESS(zeCommandListDestroy(cmdList));
    return true;
#if 0
    Opencl opencl;
    Timer timer;
    cl_int retVal;
    const auto gws = arguments.workgroupCount * arguments.workgroupSize;
    const auto lws = arguments.workgroupSize;

    // Create kernel
    const char *source = "__kernel void empty() {}";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    retVal = clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr);
    ASSERT_CL_SUCCESS(retVal);
    cl_kernel kernel = clCreateKernel(program, "empty", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup, kernel
    size_t warmupOffset = 8;
    retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, &warmupOffset, &gws, &lws, 0, nullptr, nullptr);
    retVal |= clFinish(opencl.commandQueue);
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        // Enqueue empty kernel and measure it
        timer.measureStart();
        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr);
        retVal |= clFinish(opencl.commandQueue);
        timer.measureEnd();
        ASSERT_CL_SUCCESS(retVal);
        statistics.pushValue(timer.Get());
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    return true;
#endif
}

static RegisterTestCase<EmptyKernel> registerTestCase(run, Api::L0);
} // namespace UllsTest
