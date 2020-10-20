#include "framework/ocl/compression_helper.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"
#include "multitile_memory_benchmark/definitions/fill_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const FillBufferArguments &arguments, Statistics &statistics) {
    if (arguments.compressed && arguments.noIntelExtensions) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    QueueProperties queueProperties = QueueProperties::create().setBcs(arguments.copyQueue).allowCreationFail();
    DeviceProperties deviceProperties = DeviceProperties::create().setDeviceSelection(arguments.queuePlacement).allowSubDeviceCreationFail();
    Opencl opencl(queueProperties, deviceProperties);
    if (opencl.commandQueue == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    Timer timer;
    cl_int retVal;

    // Create buffer
    const cl_context contextForBuffer = opencl.createContext(arguments.bufferPlacement);
    if (contextForBuffer == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    const cl_mem_flags compressionHint = CompressionHelper::getCompressionFlags(arguments.compressed, arguments.noIntelExtensions);
    const cl_mem buffer = clCreateBuffer(contextForBuffer, CL_MEM_READ_WRITE | compressionHint, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Check buffer compression
    const auto compressionStatus = CompressionHelper::verifyCompression(buffer, arguments.compressed, arguments.noIntelExtensions);
    if (compressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
        return compressionStatus;
    }

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

static RegisterTestCaseImplementation<FillBuffer> registerTestCase(run, Api::OpenCL);
