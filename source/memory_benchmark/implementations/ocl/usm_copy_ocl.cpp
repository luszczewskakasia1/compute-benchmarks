#include "framework/ocl/memory_placement_helper_ocl.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/ocl/profiling_helper.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/usm_copy.h"
#include "memory_benchmark/timer_helper.h"

#include <gtest/gtest.h>

static TestResult run(const UsmCopyArguments &arguments, Statistics &statistics) {
    cl_int retVal{};
    Opencl opencl(QueueProperties::createProfilingOrNot(arguments.useEvents));
    Timer timer;
    auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
    auto clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clDeviceMemAllocINTEL");
    auto clSharedMemAllocINTEL = (pfn_clSharedMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clSharedMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemcpyINTEL = (pfn_clEnqueueMemcpyINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemcpyINTEL");
    if (!clHostMemAllocINTEL || !clDeviceMemAllocINTEL || !clMemFreeINTEL || !clEnqueueMemcpyINTEL) {
        return TestResult::DriverFunctionNotFound;
    }

    // Create buffers
    void *source = clHostOrDeviceOrSharedAllocINTEL(arguments.sourcePlacement, opencl.platform, opencl.context, opencl.device, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    void *destination = clHostOrDeviceOrSharedAllocINTEL(arguments.destinationPlacement, opencl.platform, opencl.context, opencl.device, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);

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
            cl_ulong timeNs{};
            ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(profilingEvent, timeNs));
            ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
            TimerHelper::pushValue(statistics, timeNs, arguments.size);
        } else {
            TimerHelper::pushValueFromTimer(statistics, timer, arguments.size);
        }
    }

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, destination));
    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, source));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmCopy> registerTestCase(run, Api::OpenCL, true);
