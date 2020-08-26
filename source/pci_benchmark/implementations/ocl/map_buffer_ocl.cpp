#include "framework/ocl/map_flags_ocl.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/timer.h"
#include "pci_benchmark/definitions/map_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const MapBufferArguments &arguments, Statistics &statistics) {
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

    // Check buffer compression
    const auto compressionStatus = Opencl::verifyCompression(buffer, arguments.compressed, arguments.noIntelExtensions);
    if (compressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
        return compressionStatus;
    }

    // Warmup
    const auto mapFlags = convertMapFlags(arguments.mapFlags);
    void *ptr = clEnqueueMapBuffer(opencl.commandQueue, buffer, CL_BLOCKING, mapFlags, 0, arguments.size, 0, nullptr, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clEnqueueUnmapMemObject(opencl.commandQueue, buffer, ptr, 0, nullptr, nullptr));
    clFinish(opencl.commandQueue);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ptr = clEnqueueMapBuffer(opencl.commandQueue, buffer, CL_NON_BLOCKING, mapFlags, 0, arguments.size, 0, nullptr, nullptr, &retVal);
        ASSERT_CL_SUCCESS(retVal);
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();

        statistics.pushValue(timer.getBandwidth(arguments.size));

        ASSERT_CL_SUCCESS(clEnqueueUnmapMemObject(opencl.commandQueue, buffer, ptr, 0, nullptr, nullptr));
        clFinish(opencl.commandQueue);
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    return TestResult::Success;
}

static RegisterTestCase<MapBuffer> registerTestCase(run, Api::OpenCL);
