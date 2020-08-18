#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "pci_benchmark/write_buffer_rect.h"

#include <gtest/gtest.h>

static TestResult run(const WriteBufferRectArguments &arguments, Statistics &statistics) {
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
    cl_bool isCompressed{};
    ASSERT_CL_SUCCESS(clGetMemObjectInfo(buffer, CL_MEM_USES_COMPRESSION_INTEL, sizeof(isCompressed), &isCompressed, nullptr));
    if ((isCompressed == CL_TRUE) != arguments.compressed) {
        return TestResult::DeviceNotCapable;
    }

    // Warmup
    size_t bufferOffset[3] = {};
    ASSERT_CL_SUCCESS(clEnqueueWriteBufferRect(opencl.commandQueue, buffer, CL_BLOCKING,
                                               bufferOffset, arguments.origin, arguments.region,
                                               arguments.rPitch, arguments.sPitch, arguments.rPitch, arguments.sPitch,
                                               cpuBuffer.get(), 0, nullptr, nullptr));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueWriteBufferRect(opencl.commandQueue, buffer, CL_NON_BLOCKING,
                                                   bufferOffset, arguments.origin, arguments.region,
                                                   arguments.rPitch, arguments.sPitch, arguments.rPitch, arguments.sPitch,
                                                   cpuBuffer.get(), 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))
        timer.measureEnd();

        statistics.pushValue(timer.getBandwidth(arguments.size));
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    return TestResult::Success;
}

static RegisterTestCase<WriteBufferRect> registerTestCase(run, Api::OpenCL);
