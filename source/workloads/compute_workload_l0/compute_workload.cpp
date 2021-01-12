#include "framework/l0/levelzero.h"
#include "framework/utility/load_binary_file.h"
#include "framework/utility/timer.h"
#include "framework/workload/register_workload.h"

struct ComputeWorkloadParameters : WorkloadParameters {
    PositiveIntegerTestCaseArgument bufferSize;

    ComputeWorkloadParameters()
        : bufferSize(*this, "size", "Size of the buffer") {}
};

struct ComputeWorkload : Workload<ComputeWorkloadParameters> {};

TestResult run(ComputeWorkloadParameters &arguments, WorkloadStatistics &statistics) {
    LevelZero levelzero{};
    Timer timer{};

    // Create buffer
    void *buffer = nullptr;
    const ze_device_mem_alloc_desc_t deviceAllocationDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeMemAllocDevice(levelzero.context, &deviceAllocationDesc, arguments.bufferSize, 0, levelzero.device, &buffer));
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeContextMakeMemoryResident(levelzero.context, levelzero.device, buffer, arguments.bufferSize))

    // Ensure we're running on a single tile
    uint32_t tilesCount = {};
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeDeviceGetSubDevices(levelzero.device, &tilesCount, nullptr));
    if (tilesCount > 1) {
        return TestResult::DeviceNotCapable;
    }

    // Create kernel
    auto spirvModule = loadBinaryFile("compute_workload_increment.spv");
    if (spirvModule.size() == 0) {
        return TestResult::KernelNotFound;
    }
    ze_module_handle_t module{};
    ze_kernel_handle_t kernel{};
    ze_module_desc_t moduleDesc{ZE_STRUCTURE_TYPE_MODULE_DESC};
    moduleDesc.format = ZE_MODULE_FORMAT_IL_SPIRV;
    moduleDesc.pInputModule = reinterpret_cast<const uint8_t *>(spirvModule.data());
    moduleDesc.inputSize = spirvModule.size();
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeModuleCreate(levelzero.context, levelzero.device, &moduleDesc, &module, nullptr));
    ze_kernel_desc_t kernelDesc{ZE_STRUCTURE_TYPE_KERNEL_DESC};
    kernelDesc.pKernelName = "increment";
    const uint32_t groupSize = 256;
    const ze_group_count_t groupCount{static_cast<uint32_t>(arguments.bufferSize) / groupSize, 1, 1};
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeKernelCreate(module, &kernelDesc, &kernel));
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeKernelSetGroupSize(kernel, groupSize, 1u, 1u));
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeKernelSetArgumentValue(kernel, 0, sizeof(buffer), &buffer));

    // Create command list
    ze_command_list_desc_t cmdListDesc{};
    cmdListDesc.commandQueueGroupOrdinal = levelzero.commandQueueDesc.ordinal;
    ze_command_list_handle_t cmdList{};
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeCommandListCreate(levelzero.context, levelzero.device, &cmdListDesc, &cmdList));
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeCommandListAppendLaunchKernel(cmdList, kernel, &groupCount, nullptr, 0, nullptr));
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeCommandListClose(cmdList));

    // Warmup
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint64_t>::max()));

    // Benchmark
    for (auto i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueExecuteCommandLists(levelzero.commandQueue, 1, &cmdList, nullptr));
        ASSERT_ZE_RESULT_SUCCESS(zeCommandQueueSynchronize(levelzero.commandQueue, std::numeric_limits<uint32_t>::max()));
        timer.measureEnd();

        statistics.pushValue(timer.get());
    }

    // Evict buffer
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeContextEvictMemory(levelzero.context, levelzero.device, buffer, arguments.bufferSize));

    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeCommandListDestroy(cmdList));
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeKernelDestroy(kernel));
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeModuleDestroy(module));
    ZE_RESULT_SUCCESS_OR_RETURN_ERROR(zeMemFree(levelzero.context, buffer));
    return TestResult::Success;
}

static RegisterWorkloadImplementation<ComputeWorkload> registerImplementation{run};

int main(int argc, char **argv) {
    ComputeWorkload workload;
    return workload.runFromCommandLine(argc, argv);
}
