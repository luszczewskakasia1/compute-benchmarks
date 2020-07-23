#include "framework/opencl.h"
#include "framework/register_test_case.h"
#include "framework/timer.h"
#include "tests/new_resources_submission.h"

#include <gtest/gtest.h>

namespace UllsTest {

static bool run(const NewResourcesSubmissionArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    cl_int retVal;

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

    // Warmup kernel
    const size_t gws = 1;
    const size_t sizeInBytes = arguments.sizeMB * 1024 * 1024;
    cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, sizeInBytes, nullptr, &retVal);
    retVal |= clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
    retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr);
    retVal |= clFinish(opencl.commandQueue);
    retVal |= clReleaseMemObject(buffer);
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, sizeInBytes, nullptr, &retVal);
        retVal |= clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
        ASSERT_CL_SUCCESS(retVal);

        timer.measureStart();
        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr);
        retVal |= clFlush(opencl.commandQueue);
        timer.messureEnd();
        ASSERT_CL_SUCCESS(retVal);

        retVal |= clFinish(opencl.commandQueue);
        retVal |= clReleaseMemObject(buffer);
        ASSERT_CL_SUCCESS(retVal);

        statistics.pushValue(timer.Get());
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    return true;
}

static RegisterTestCase<NewResourcesSubmission> registerTestCase(run, Api::OpenCL);
} // namespace UllsTest
