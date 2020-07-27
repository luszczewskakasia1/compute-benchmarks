#include "framework/opencl.h"
#include "framework/register_test_case.h"
#include "framework/timer.h"
#include "tests/new_resources_submission_host.h"

#include <gtest/gtest.h>

namespace UllsTest {

static TestResult run(const NewResourcesSubmissionHostArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    if (!clHostMemAllocINTEL || !clMemFreeINTEL) {
        return TestResult::DriverFunctionNotFound;
    }
    cl_int retVal;

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

    // Warmup kernel
    const size_t gws = 1;
    const size_t sizeInBytes = arguments.size;
    int *hostMemory = (int *)clHostMemAllocINTEL(opencl.context, nullptr, 64, 0, &retVal);
    ASSERT_CL_SUCCESS(clSetKernelArgSVMPointer(kernel, 0, hostMemory));
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, hostMemory));
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        int *hostMemory = (int *)clHostMemAllocINTEL(opencl.context, nullptr, sizeInBytes, 0, &retVal);
        ASSERT_CL_SUCCESS(clSetKernelArgSVMPointer(kernel, 0, hostMemory));
        retVal |= clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr);
        retVal |= clFinish(opencl.commandQueue);
        timer.measureEnd();

        ASSERT_CL_SUCCESS(retVal);
        ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, hostMemory));

        statistics.pushValue(timer.Get());
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    return TestResult::Success;
}

static RegisterTestCase<NewResourcesSubmissionHost> registerTestCase(run, Api::OpenCL);
} // namespace UllsTest
