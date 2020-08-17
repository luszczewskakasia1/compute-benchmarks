#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "pci_benchmark/copy_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const CopyBufferArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    cl_int retVal;

    // Create buffer
    const cl_mem_flags compressionHint = arguments.compressed ? CL_MEM_COMPRESSED_HINT_INTEL : CL_MEM_UNCOMPRESSED_HINT_INTEL;
    const cl_mem_flags memFlags = CL_MEM_READ_WRITE | compressionHint;
    const cl_mem source = clCreateBuffer(opencl.context, memFlags, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    const cl_mem destination = clCreateBuffer(opencl.context, memFlags, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    auto cpuBuffer = std::make_unique<uint8_t[]>(arguments.size);

    // Check buffers compression
    //cl_bool isSrcCompressed{};
    //cl_bool isDstCompressed{};
    //ASSERT_CL_SUCCESS(clGetMemObjectInfo(source, CL_MEM_USES_COMPRESSION_INTEL, sizeof(isSrcCompressed), &isSrcCompressed, nullptr));
    //ASSERT_CL_SUCCESS(clGetMemObjectInfo(destination, CL_MEM_USES_COMPRESSION_INTEL, sizeof(isDstCompressed), &isDstCompressed, nullptr));
    //if ((isSrcCompressed == CL_TRUE) != arguments.compressed && (isDstCompressed == CL_TRUE) != arguments.compressed) {
    //    return TestResult::DeviceNotCapable;
    //}

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueCopyBuffer(opencl.commandQueue, source, destination, 0, 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueCopyBuffer(opencl.commandQueue, source, destination, 0, 0, arguments.size, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();

        statistics.pushValue(timer.getBandwidth(arguments.size));
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(destination));
    ASSERT_CL_SUCCESS(clReleaseMemObject(source));
    return TestResult::Success;
}

static RegisterTestCase<CopyBuffer> registerTestCase(run, Api::OpenCL);
