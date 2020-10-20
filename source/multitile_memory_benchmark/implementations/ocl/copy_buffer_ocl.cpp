#include "framework/ocl/compression_helper.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/ocl/profiling_helper.h"
#include "framework/utility/timer.h"
#include "multitile_memory_benchmark/definitions/copy_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const CopyBufferArguments &arguments, Statistics &statistics) {
    if ((arguments.dstCompressed || arguments.srcCompressed) && arguments.noIntelExtensions) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    cl_int retVal;
    QueueProperties queueProperties = QueueProperties::create().setProfiling(arguments.useEvents);
    DeviceProperties devicePropertes = DeviceProperties::create().setDeviceSelection(arguments.queuePlacement).allowSubDeviceCreationFail();
    Opencl opencl(queueProperties, devicePropertes);
    if (opencl.device == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    Timer timer;

    // Get contexts for buffers
    const cl_context srcContext = opencl.createContext(arguments.srcPlacement);
    const cl_context dstContext = opencl.createContext(arguments.dstPlacement);
    if (srcContext == nullptr || dstContext == nullptr) {
        return TestResult::DeviceNotCapable;
    }

    // Create buffer
    const cl_mem_flags compressionHintSrc = CompressionHelper::getCompressionFlags(arguments.srcCompressed, arguments.noIntelExtensions);
    const cl_mem_flags compressionHintDst = CompressionHelper::getCompressionFlags(arguments.dstCompressed, arguments.noIntelExtensions);
    const cl_mem source = clCreateBuffer(srcContext, CL_MEM_READ_WRITE | compressionHintSrc, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    const cl_mem destination = clCreateBuffer(dstContext, CL_MEM_READ_WRITE | compressionHintDst, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Check buffers compression
    const auto srcCompressionStatus = CompressionHelper::verifyCompression(source, arguments.srcCompressed, arguments.noIntelExtensions);
    const auto dstCompressionStatus = CompressionHelper::verifyCompression(destination, arguments.dstCompressed, arguments.noIntelExtensions);
    if (srcCompressionStatus != TestResult::Success || dstCompressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(source));
        ASSERT_CL_SUCCESS(clReleaseMemObject(destination));
        if (srcCompressionStatus != TestResult::Success) {
            return srcCompressionStatus;
        } else {
            return dstCompressionStatus;
        }
    }

    // Fill buffers
    const char pattern[] = {0};
    ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, source, pattern, sizeof(pattern) / sizeof(pattern[0]), 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, destination, pattern, sizeof(pattern) / sizeof(pattern[0]), 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueCopyBuffer(opencl.commandQueue, source, destination, 0, 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueCopyBuffer(opencl.commandQueue, source, destination, 0, 0, arguments.size, 0, nullptr, eventForEnqueue));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();

        if (eventForEnqueue) {
            cl_ulong timeNs{};
            ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(profilingEvent, timeNs));
            ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
            statistics.pushValue(Timer::getBandwidth(static_cast<double>(timeNs), arguments.size));
        } else {
            statistics.pushValue(timer.getBandwidth(arguments.size));
        }
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(destination));
    ASSERT_CL_SUCCESS(clReleaseMemObject(source));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<CopyBuffer> registerTestCase(run, Api::OpenCL);
