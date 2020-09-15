
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"
#include "ulls_benchmark/definitions/walker_submission_events.h"

#include <emmintrin.h>
#include <gtest/gtest.h>

static TestResult run(const WalkerSubmissionEventsArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl(false);
    cl_int retVal{};
    cl_command_queue commandQueue = clCreateCommandQueueWithProperties(opencl.context, opencl.device, opencl.profilingQueueProperties, &retVal);
    EXPECT_CL_SUCCESS(retVal);
    cl_event profilingEvent{};

    // Create kernel
    const char *source = "__kernel void empty() {}";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "empty", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup run
    const size_t gws = 1;
    const size_t lws = 1;
    retVal |= clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, &profilingEvent);
    retVal |= clFinish(commandQueue);
    ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, &profilingEvent));
        ASSERT_CL_SUCCESS(clWaitForEvents(1, &profilingEvent));

        cl_ulong queued{}, start{};
        ASSERT_CL_SUCCESS(clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, nullptr));
        ASSERT_CL_SUCCESS(clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &queued, nullptr));
        ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
        const auto timeNs = static_cast<Statistics::Value>(start - queued);
        const auto timeUs = timeNs / 1000;

        statistics.pushValue(timeUs);
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clReleaseCommandQueue(commandQueue));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<WalkerSubmissionEvents> registerTestCase(run, Api::OpenCL);
