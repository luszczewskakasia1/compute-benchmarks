#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "ulls_benchmark/definitions/flush_time.h"

#include <gtest/gtest.h>

static TestResult run(const FlushTimeArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    cl_int retVal;

    // Get parameters for the enqueue call
    cl_event event{};
    cl_event *eventForNdr = arguments.useEvent ? &event : nullptr;
    size_t gws = arguments.workgroupCount * arguments.workgroupSize;
    const size_t lws = arguments.workgroupSize;
    const size_t *lwsForNdr = (lws != 0) ? &lws : nullptr;

    if (gws == 0) {
        gws = 1;
    }

    // Create kernel
    const char *source = "__kernel void empty() {}";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "empty", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup, kernel
    retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, lwsForNdr, 0, nullptr, eventForNdr);
    retVal |= clFinish(opencl.commandQueue);
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, lwsForNdr, 0, nullptr, eventForNdr));
        timer.measureStart();
        ASSERT_CL_SUCCESS(clFlush(opencl.commandQueue));
        timer.measureEnd();
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));        
        statistics.pushValue(timer.Get());
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    if (eventForNdr) {
        ASSERT_CL_SUCCESS(clReleaseEvent(event));
    }
    return TestResult::Success;
}

static RegisterTestCase<FlushTime> registerTestCase(run, Api::OpenCL);
