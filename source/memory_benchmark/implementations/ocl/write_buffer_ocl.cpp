#include "framework/ocl/compression_helper.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/ocl/buffer_contents_helper_ocl.h"
#include "framework/utility/ocl/profiling_helper.h"
#include "framework/utility/timer.h"
#include "memory_benchmark/definitions/write_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const WriteBufferArguments &arguments, Statistics &statistics) {
    if (arguments.compressed && arguments.noIntelExtensions) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    QueueProperties queueProperties = QueueProperties::create().setProfiling(arguments.useEvents);
    Opencl opencl(queueProperties);
    Timer timer;
    cl_int retVal;

    // Create buffer
    const cl_mem_flags compressionHint = CompressionHelper::getCompressionFlags(arguments.compressed, arguments.noIntelExtensions);
    const cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE | compressionHint, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    auto cpuBuffer = std::make_unique<uint8_t[]>(arguments.size);

    // Check buffer compression
    const auto compressionStatus = CompressionHelper::verifyCompression(buffer, arguments.compressed, arguments.noIntelExtensions);
    if (compressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
        return compressionStatus;
    }

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueWriteBuffer(opencl.commandQueue, buffer, CL_BLOCKING, 0, arguments.size, cpuBuffer.get(), 0, nullptr, nullptr));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        ASSERT_CL_SUCCESS(BufferContentsHelperOcl::fillBuffer(opencl.commandQueue, buffer, arguments.size, arguments.contents));

        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueWriteBuffer(opencl.commandQueue, buffer, CL_NON_BLOCKING, 0, arguments.size, cpuBuffer.get(), 0, nullptr, eventForEnqueue));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))
        timer.measureEnd();

        if (eventForEnqueue) {
            cl_ulong timeNs{};
            ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(profilingEvent, timeNs));
            ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
            statistics.pushValue(std::chrono::nanoseconds(timeNs), arguments.size);
        } else {
            statistics.pushValue(timer.get(), arguments.size);
        }
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<WriteBuffer> registerTestCase(run, Api::OpenCL);
