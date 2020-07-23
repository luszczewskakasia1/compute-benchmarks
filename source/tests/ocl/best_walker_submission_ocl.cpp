#include "framework/opencl.h"
#include "framework/register_test_case.h"
#include "framework/timer.h"
#include "tests/best_walker_submission.h"

#include <gtest/gtest.h>

namespace UllsTest {
static bool run(const BestWalkerSubmissionArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    cl_int retVal;

    // Create system memory buffer
    int *hostMemory = (int *)clHostMemAllocINTEL(opencl.context, nullptr, 64, 0, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Create kernel
    const char *source = "__kernel void write(__global int *outBuffer) {  \n"
                         "   outBuffer[get_global_id(0)] = 1;             \n"
                         "}";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    retVal = clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr);
    ASSERT_CL_SUCCESS(retVal);
    cl_kernel kernel = clCreateKernel(program, "write", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    ASSERT_CL_SUCCESS(retVal);
    retVal = clSetKernelArgSVMPointer(kernel, 0, hostMemory);
    size_t gws = 1;

    for (int i = 0; i < arguments.iterations; i++) {
        // Reset value
        *hostMemory = 0;

        // Warmup, kernel
        size_t warmupOffset = 8;
        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, &warmupOffset, &gws, nullptr, 0, nullptr, nullptr);
        retVal |= clFinish(opencl.commandQueue);
        ASSERT_CL_SUCCESS(retVal);

        // Enqueue write on GPU and poll for update on CPU
        timer.measureStart();
        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr);
        retVal |= clFlush(opencl.commandQueue);
        ASSERT_CL_SUCCESS(retVal);

        while (*hostMemory != 1) {
        }
        timer.measureEnd();
        statistics.pushValue(timer.Get());
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, hostMemory));
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    return true;
}

static RegisterTestCase<BestWalkerSubmission> registerTestCase(run, Api::OpenCL);
} // namespace UllsTest
