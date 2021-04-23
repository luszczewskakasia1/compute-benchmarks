#include "framework/ocl/opencl.h"
#include "framework/ocl/utility/buffer_contents_helper_ocl.h"
#include "framework/ocl/utility/profiling_helper.h"
#include "framework/ocl/utility/usm_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/usm_copy.h"

#include <gtest/gtest.h>

static TestResult run(const UsmCopyArguments &arguments, Statistics &statistics) {
    // Setup
    cl_int retVal{};
    QueueProperties queueProperties = QueueProperties::create().setProfiling(arguments.useEvents).setForceBlitter(arguments.forceBlitter).allowCreationFail();
    Opencl opencl(queueProperties);
    if (opencl.commandQueue == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    if (!QueueFamiliesHelper::validateCapabilities(opencl.commandQueue, CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL, CL_QUEUE_CAPABILITY_FILL_BUFFER_INTEL)) {
        return TestResult::DeviceNotCapable;
    }
    Timer timer;
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemcpyINTEL = (pfn_clEnqueueMemcpyINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemcpyINTEL");
    if (!opencl.getExtensions().isUsmSupported()) {
        return TestResult::DriverFunctionNotFound;
    }

    // Create buffers
    void *source = UsmHelper::allocate(arguments.sourcePlacement, opencl.platform, opencl.context, opencl.device, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    void *destination = UsmHelper::allocate(arguments.destinationPlacement, opencl.platform, opencl.context, opencl.device, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueMemcpyINTEL(opencl.commandQueue, CL_FALSE, destination, source, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        if (arguments.sourcePlacement == UsmMemoryPlacement::NonUsm) {
            std::memset(source, 0, arguments.size);
        } else {
            ASSERT_CL_SUCCESS(BufferContentsHelperOcl::fillUsmBuffer(opencl.commandQueue, source, arguments.size, arguments.contents));
        }
        if (arguments.destinationPlacement == UsmMemoryPlacement::NonUsm) {
            std::memset(destination, 0, arguments.size);
        } else {
            ASSERT_CL_SUCCESS(BufferContentsHelperOcl::fillUsmBuffer(opencl.commandQueue, source, arguments.size, arguments.contents));
        }

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
            statistics.pushValue(std::chrono::nanoseconds(timeNs), arguments.size);
        } else {
            statistics.pushValue(timer.get(), arguments.size);
        }
    }
    if (arguments.sourcePlacement == UsmMemoryPlacement::NonUsm) {
        free(source);
    } else {
        ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, source));
    }
    if (arguments.destinationPlacement == UsmMemoryPlacement::NonUsm) {
        free(destination);
    } else {
        ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, destination));
    }

    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmCopy> registerTestCase(run, Api::OpenCL, true);
