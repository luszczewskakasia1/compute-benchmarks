#include "framework/ocl/compression_helper.h"
#include "framework/ocl/opencl.h"
#include "framework/ocl/profiling_helper.h"
#include "framework/ocl/usm_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"
#include "multitile_memory_benchmark/definitions/usm_copy_kernel.h"

#include <gtest/gtest.h>

static TestResult run(const UsmCopyKernelArguments &arguments, Statistics &statistics) {
    // Setup
    cl_int retVal;
    QueueProperties queueProperties = QueueProperties::create().setDeviceSelection(arguments.queuePlacement).setForceBlitter(arguments.forceBlitter).setProfiling(arguments.useEvents).allowCreationFail();
    ContextProperties contextProperties = ContextProperties::create().setDeviceSelection(arguments.contextPlacement).allowCreationFail();
    Opencl opencl(queueProperties, contextProperties);
    if (opencl.context == nullptr || opencl.commandQueue == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    auto clCreateBufferWithPropertiesINTEL = (pfn_clCreateBufferWithPropertiesINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clCreateBufferWithPropertiesINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemcpyINTEL = (pfn_clEnqueueMemcpyINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemcpyINTEL");
    if (!clCreateBufferWithPropertiesINTEL || !opencl.getExtensions().isUsmSupported()) {
        return TestResult::DriverFunctionNotFound;
    }
    Timer timer;

    // Create buffer
    void *src = UsmHelper::allocate(arguments.srcPlacement, opencl, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    void *dst = UsmHelper::allocate(arguments.dstPlacement, opencl, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Create kernel
    const char *source = "kernel void copy_buffer(__global int *src, __global int *dst) { const uint gid = get_global_id(0);  dst[gid] = src[gid]; }";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 0u, nullptr, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "copy_buffer", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup
    const size_t gws = arguments.size / sizeof(cl_int);
    ASSERT_CL_SUCCESS(clSetKernelArgSVMPointer(kernel, 0, src));
    ASSERT_CL_SUCCESS(clSetKernelArgSVMPointer(kernel, 1, dst));
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, eventForEnqueue));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))
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

    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, src));
    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, dst));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmCopyKernel> registerTestCase(run, Api::OpenCL, true);
