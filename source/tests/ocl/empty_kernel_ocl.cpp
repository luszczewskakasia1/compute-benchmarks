#include "framework/ocl/opencl.h"
#include "framework/register_test_case.h"
#include "framework/timer.h"
#include "tests/empty_kernel.h"

#include <gtest/gtest.h>

namespace UllsTest {

static TestResult run(const EmptyKernelArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    cl_int retVal;
    const auto gws = arguments.workgroupCount * arguments.workgroupSize;
    const auto lws = arguments.workgroupSize;

    // Create kernel
    const char *source = "__kernel void empty() {}";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "empty", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup, kernel
    size_t warmupOffset = 8;
    retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, &warmupOffset, &gws, &lws, 0, nullptr, nullptr);
    retVal |= clFinish(opencl.commandQueue);
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        // Enqueue empty kernel and measure it
        timer.measureStart();
        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr);
        retVal |= clFinish(opencl.commandQueue);
        timer.measureEnd();
        ASSERT_CL_SUCCESS(retVal);
        statistics.pushValue(timer.Get());
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    return TestResult::Success;
}

static RegisterTestCase<EmptyKernel> registerTestCase(run, Api::OpenCL);
} // namespace UllsTest
