#include "atomic_benchmark/definitions/one_atomic.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/load_binary_file.h"
#include "framework/utility/timer.h"

#include <gtest/gtest.h>

static TestResult run(const OneAtomicArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl{};
    Timer timer{};
    cl_int retVal{};

    // Create buffer
    cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, sizeof(cl_uint), nullptr, &retVal);

    // Create kernel
    const std::vector<uint8_t> kernelSource = loadBinaryFile("atomic_benchmark_kernel.cl");
    if (kernelSource.size() == 0) {
        return TestResult::KernelNotFound;
    }
    const char *source = reinterpret_cast<const char *>(kernelSource.data());
    const size_t sourceLength = kernelSource.size();
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    const std::string compilerOptions = AtomicOperationHelper::getCompilerOptions(arguments.atomicOperation);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, compilerOptions.c_str(), nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "one_atomic", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup
    const size_t lws = arguments.workgroupSize;
    const size_t gws = arguments.workgroupSize * arguments.workgroupCount;
    const cl_uint otherArgument = 1;
    const cl_uint loopIterations = 100;
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(buffer), &buffer));
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 1, sizeof(otherArgument), &otherArgument));
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 2, sizeof(loopIterations), &loopIterations));
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();
        statistics.pushValue(timer.get());
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<OneAtomic> registerTestCase(run, Api::OpenCL);
