#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "memory_benchmark/definitions/read_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const ReadBufferArguments &arguments, Statistics &statistics) {
    if (arguments.compressed && arguments.noIntelExtensions) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    Opencl opencl;
    Timer timer;
    cl_int retVal;

    // Create buffer
    const cl_mem_flags compressionHint = Opencl::getCompressionFlags(arguments.compressed, arguments.noIntelExtensions);
    const cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE | compressionHint, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    auto cpuBuffer = std::make_unique<uint8_t[]>(arguments.size);

    // Check buffer compression
    const auto compressionStatus = Opencl::verifyCompression(buffer, arguments.compressed, arguments.noIntelExtensions);
    if (compressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
        return compressionStatus;
    }

    // Fill buffers
    const char pattern[] = {0};
    ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, buffer, pattern, sizeof(pattern) / sizeof(pattern[0]), 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueReadBuffer(opencl.commandQueue, buffer, CL_BLOCKING, 0, arguments.size, cpuBuffer.get(), 0, nullptr, nullptr));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueReadBuffer(opencl.commandQueue, buffer, CL_NON_BLOCKING, 0, arguments.size, cpuBuffer.get(), 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))
        timer.measureEnd();

        statistics.pushValue(timer.getBandwidth(arguments.size));
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<ReadBuffer> RegisterTestCaseImplementation(run, Api::OpenCL);
