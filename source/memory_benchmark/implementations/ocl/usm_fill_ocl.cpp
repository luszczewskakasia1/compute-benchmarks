#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/ocl/profiling_helper.h"
#include "framework/utility/random_helper.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/usm_fill.h"

#include <gtest/gtest.h>

static TestResult run(const UsmFillArguments &arguments, Statistics &statistics) {
    // Setup
    cl_int retVal{};
    const auto queueProperties = arguments.useEvents ? Opencl::profilingQueueProperties : Opencl::queueProperties;
    Opencl opencl(queueProperties);
    Timer timer;
    auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
    auto clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clDeviceMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemFillINTEL = (pfn_clEnqueueMemFillINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemFillINTEL");
    if (!clHostMemAllocINTEL || !clDeviceMemAllocINTEL || !clMemFreeINTEL || !clEnqueueMemFillINTEL) {
        return TestResult::DriverFunctionNotFound;
    }

    // Create buffers
    void *buffer{};
    if (isDeviceMemory(arguments.memoryPlacement)) {
        buffer = clDeviceMemAllocINTEL(opencl.context, opencl.device, nullptr, arguments.bufferSize, 0u, &retVal);
    } else {
        buffer = clHostMemAllocINTEL(opencl.context, nullptr, arguments.bufferSize, 0u, &retVal);
    }
    ASSERT_CL_SUCCESS(retVal);

    // Create pattern
    const auto pattern = std::make_unique<uint8_t[]>(arguments.patternSize);
    if (arguments.patternContents == BufferContents::Random) {
        RandomHelper::fillWithRandomBytes(pattern.get(), arguments.patternSize);
    }

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueMemFillINTEL(opencl.commandQueue, buffer, pattern.get(), arguments.patternSize, arguments.bufferSize, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueMemFillINTEL(opencl.commandQueue, buffer, pattern.get(), arguments.patternSize, arguments.bufferSize, 0, nullptr, eventForEnqueue));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();

        if (eventForEnqueue) {
            cl_ulong timeNs{};
            ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(profilingEvent, timeNs));
            ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
            statistics.pushValue(Timer::getBandwidth(static_cast<Statistics::Value>(timeNs), arguments.bufferSize));
        } else {
            statistics.pushValue(timer.getBandwidth(arguments.bufferSize));
        }
    }

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmFill> registerTestCase(run, Api::OpenCL, true);
