
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "ulls_benchmark/walker_submission_events.h"

#include <emmintrin.h>
#include <gtest/gtest.h>

static TestResult run(const WalkerSubmissionEventsArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl(false);
    cl_int retVal{};
    const cl_queue_properties queueProperties[] = {CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0};
    cl_command_queue commandQueue = clCreateCommandQueueWithProperties(opencl.context, opencl.device, queueProperties, &retVal);
    EXPECT_CL_SUCCESS(retVal);
    cl_event profilingEvent{};

    // Create system memory buffer
    cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, 64, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Create kernel
    const char *source = "__kernel void write(__global int *outBuffer) {  \n"
                         "   outBuffer[get_global_id(0)] = 1;             \n"
                         "}";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "write", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup run
    const size_t gws = 1;
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(buffer), &buffer));
    retVal |= clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, &profilingEvent);
    retVal |= clFinish(commandQueue);
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, &profilingEvent));
        ASSERT_CL_SUCCESS(clWaitForEvents(1, &profilingEvent));

        cl_ulong queued{}, start{};
        ASSERT_CL_SUCCESS(clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, nullptr));
        ASSERT_CL_SUCCESS(clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &queued, nullptr));
        const auto timeNs = static_cast<Statistics::Value>(start - queued);
        const auto timeUs = timeNs / 1000;

        statistics.pushValue(timeUs);
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clReleaseCommandQueue(commandQueue));
    return TestResult::Success;
}

static RegisterTestCase<WalkerSubmissionEvents> registerTestCase(run, Api::OpenCL);
