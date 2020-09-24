#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/usm_copy.h"

#include <gtest/gtest.h>

static TestResult run(const UsmCopyArguments &arguments, Statistics &statistics) {
    cl_int retVal{};
    Opencl opencl(false);
    const auto queueProperties = arguments.useEvents ? opencl.profilingQueueProperties : opencl.queueProperties;
    opencl.commandQueue = clCreateCommandQueueWithProperties(opencl.context, opencl.device, queueProperties, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    Timer timer;
    auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
    auto clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clDeviceMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemcpyINTEL = (pfn_clEnqueueMemcpyINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemcpyINTEL");
    if (!clHostMemAllocINTEL || !clDeviceMemAllocINTEL || !clMemFreeINTEL || !clEnqueueMemcpyINTEL) {
        return TestResult::DriverFunctionNotFound;
    }

    // Create buffers
    void *source{}, *destination{};
    if (isDeviceMemory(arguments.transferDirection, TransferOperand::Source)) {
        source = clDeviceMemAllocINTEL(opencl.context, opencl.device, nullptr, arguments.size, 0u, &retVal);
    } else {
        source = clHostMemAllocINTEL(opencl.context, nullptr, arguments.size, 0u, &retVal);
    }
    if (isDeviceMemory(arguments.transferDirection, TransferOperand::Destination)) {
        destination = clDeviceMemAllocINTEL(opencl.context, opencl.device, nullptr, arguments.size, 0u, &retVal);
    } else {
        destination = clHostMemAllocINTEL(opencl.context, nullptr, arguments.size, 0u, &retVal);
    }

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueMemcpyINTEL(opencl.commandQueue, CL_FALSE, destination, source, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueMemcpyINTEL(opencl.commandQueue, CL_FALSE, destination, source, arguments.size, 0, nullptr, eventForEnqueue));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();

        if (eventForEnqueue) {
            cl_ulong start{}, end{};
            ASSERT_CL_SUCCESS(clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, nullptr));
            ASSERT_CL_SUCCESS(clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, nullptr));
            ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
            const auto timeNs = static_cast<Statistics::Value>(end - start);
            statistics.pushValue(Timer::getBandwidth(timeNs, arguments.size));
        } else {
            statistics.pushValue(timer.getBandwidth(arguments.size));
        }
    }

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, destination));
    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, source));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmCopy> registerTestCase(run, Api::OpenCL, true);
