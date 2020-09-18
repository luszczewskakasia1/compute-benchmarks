#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/copy_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const CopyBufferArguments &arguments, Statistics &statistics) {
    if ((arguments.compressedDestination || arguments.compressedSource) && arguments.noIntelExtensions) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    cl_int retVal;
    Opencl opencl(false);
    const auto queueProperties = arguments.useEvents ? opencl.profilingQueueProperties : opencl.queueProperties;
    opencl.commandQueue = clCreateCommandQueueWithProperties(opencl.context, opencl.device, queueProperties, &retVal);
    Timer timer;

    // Create buffer
    auto tryCompression = arguments.compressedDestination | arguments.compressedSource;
    const cl_mem_flags compressionOnHint = Opencl::getCompressionFlags(true, arguments.noIntelExtensions);
    const cl_mem_flags compressionOffHint = Opencl::getCompressionFlags(false, arguments.noIntelExtensions);

    const cl_mem_flags memFlagsWithCompression = CL_MEM_READ_WRITE | compressionOnHint;
    const cl_mem_flags memFlagsWithoutCompression = CL_MEM_READ_WRITE | compressionOffHint;

    const cl_mem source = clCreateBuffer(opencl.context, arguments.compressedSource ? memFlagsWithCompression : memFlagsWithoutCompression, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    const cl_mem destination = clCreateBuffer(opencl.context, arguments.compressedDestination ? memFlagsWithCompression : memFlagsWithoutCompression, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Check buffers compression
    auto compressionStatus = Opencl::verifyCompression(source, arguments.compressedSource, arguments.noIntelExtensions);
    if (compressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(source));
        return compressionStatus;
    }
    compressionStatus = Opencl::verifyCompression(destination, arguments.compressedDestination, arguments.noIntelExtensions);
    if (compressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(source));
        ASSERT_CL_SUCCESS(clReleaseMemObject(destination));
        return compressionStatus;
    }

    // Fill buffers
    const char pattern[] = {0};
    ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, source, pattern, sizeof(pattern) / sizeof(pattern[0]), 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, destination, pattern, sizeof(pattern) / sizeof(pattern[0]), 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueCopyBuffer(opencl.commandQueue, source, destination, 0, 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueCopyBuffer(opencl.commandQueue, source, destination, 0, 0, arguments.size, 0, nullptr, eventForEnqueue));
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

    ASSERT_CL_SUCCESS(clReleaseMemObject(destination));
    ASSERT_CL_SUCCESS(clReleaseMemObject(source));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<CopyBuffer> registerTestCase(run, Api::OpenCL);
