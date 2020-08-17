#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "pci_benchmark/fill_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const FillBufferArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    cl_int retVal;

    // Create buffer
    const cl_mem_flags compressionHint = arguments.compressed ? CL_MEM_COMPRESSED_HINT_INTEL : CL_MEM_UNCOMPRESSED_HINT_INTEL;
    const cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE | compressionHint, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    auto cpuBuffer = std::make_unique<uint8_t[]>(arguments.size);

    // Check buffer compression
    //cl_bool isCompressed{};
    //ASSERT_CL_SUCCESS(clGetMemObjectInfo(buffer, CL_MEM_USES_COMPRESSION_INTEL, sizeof(isCompressed), &isCompressed, nullptr));
    //if ((isCompressed == CL_TRUE) != arguments.compressed) {
    //    return TestResult::DeviceNotCapable;
    //}

    // Create pattern
    const auto pattern = std::make_unique<uint8_t[]>(arguments.patternSize);

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, buffer, pattern.get(), arguments.patternSize, 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, buffer, pattern.get(), arguments.patternSize, 0, arguments.size, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))
        timer.measureEnd();

        statistics.pushValue(timer.getBandwidth(arguments.size));
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    return TestResult::Success;
}

static RegisterTestCase<FillBuffer> registerTestCase(run, Api::OpenCL);
