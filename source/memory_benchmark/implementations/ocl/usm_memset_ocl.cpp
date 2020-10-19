#include "framework/ocl/memory_placement_helper_ocl.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/usm_memset.h"
#include "memory_benchmark/timer_helper.h"

#include <gtest/gtest.h>

static TestResult run(const UsmMemsetArguments &arguments, Statistics &statistics) {
    // Setup
    QueueProperties queueProperties = QueueProperties::create().setBcs(arguments.copyQueue);
    Opencl opencl(queueProperties, false);
    if (opencl.commandQueue == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    Timer timer;
    auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
    auto clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clDeviceMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemsetINTEL = (pfn_clEnqueueMemsetINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemsetINTEL");
    if (!clHostMemAllocINTEL || !clDeviceMemAllocINTEL || !clMemFreeINTEL || !clEnqueueMemsetINTEL) {
        return TestResult::DriverFunctionNotFound;
    }
    cl_int retVal;

    // Create buffer
    void *buffer = clHostOrDeviceOrSharedAllocINTEL(arguments.memoryPlacement, opencl.platform, opencl.context, opencl.device, arguments.bufferSize, &retVal);
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
        TimerHelper::pushValueFromTimer(statistics, timer, arguments.bufferSize);
    }

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmMemset> registerTestCase(run, Api::OpenCL, true);
