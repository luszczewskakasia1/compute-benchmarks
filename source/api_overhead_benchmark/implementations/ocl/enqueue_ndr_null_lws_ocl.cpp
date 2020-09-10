#include "api_overhead_benchmark/definitions/enqueue_ndr_null_lws.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"

#include <gtest/gtest.h>

static TestResult run(const EnqueueNdrNullLwsArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl(false);
    cl_int retVal{};
    cl_command_queue commandQueue = clCreateCommandQueueWithProperties(opencl.context, opencl.device, arguments.useProfiling ? opencl.profilingQueueProperties : opencl.queueProperties, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    Timer timer;

    // Get parameters for the enqueue call
    cl_event event{};
    cl_event *eventForNdr = arguments.useEvent ? &event : nullptr;
    const size_t gws = arguments.gws;

    // Create kernel
    const char *source = "__kernel void empty() {}";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "empty", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup, kernel
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, eventForNdr));
    ASSERT_CL_SUCCESS(clFinish(commandQueue));
    if (eventForNdr) {
        ASSERT_CL_SUCCESS(clReleaseEvent(event));
    }

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, eventForNdr));
        timer.measureEnd();
        ASSERT_CL_SUCCESS(clFinish(commandQueue));
        statistics.pushValue(timer.Get());
        if (eventForNdr) {
            ASSERT_CL_SUCCESS(clReleaseEvent(event));
        }
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clReleaseCommandQueue(commandQueue));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<EnqueueNdrNullLws> RegisterTestCaseImplementation(run, Api::OpenCL);
