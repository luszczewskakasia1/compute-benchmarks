#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "memory_benchmark/definitions/usm_memset.h"

#include <gtest/gtest.h>

static TestResult run(const UsmMemsetArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
    auto clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clDeviceMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemsetINTEL = (pfn_clEnqueueMemsetINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemsetINTEL");
    if (!clHostMemAllocINTEL || !clDeviceMemAllocINTEL || !clMemFreeINTEL || !clEnqueueMemsetINTEL) {
        return TestResult::DriverFunctionNotFound;
    }
    cl_int retVal;

    // Create buffers
    void *buffer{};
    if (isDeviceMemory(arguments.memoryPlacement)) {
        buffer = clDeviceMemAllocINTEL(opencl.context, opencl.device, nullptr, arguments.bufferSize, 0u, &retVal);
    } else {
        buffer = clHostMemAllocINTEL(opencl.context, nullptr, arguments.bufferSize, 0u, &retVal);
    }
    ASSERT_CL_SUCCESS(retVal);

    // Warmup
    const uint8_t memsetValue = 0x1;
    ASSERT_CL_SUCCESS(clEnqueueMemsetINTEL(opencl.commandQueue, buffer, memsetValue, arguments.bufferSize, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueMemsetINTEL(opencl.commandQueue, buffer, memsetValue, arguments.bufferSize, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();
        statistics.pushValue(timer.getBandwidth(arguments.bufferSize));
    }

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));
    return TestResult::Success;
}

static RegisterTestCase<UsmMemset> registerTestCase(run, Api::OpenCL, true);
