#include "framework/ocl/opencl.h"
#include "framework/ocl/utility/profiling_helper.h"
#include "framework/ocl/utility/usm_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/usm_copy_multiple_blits.h"

#include <gtest/gtest.h>

static TestResult run(const UsmCopyMultipleBlitsArguments &arguments, Statistics &statistics) {
    // Setup
    cl_int retVal{};
    QueueProperties queueProperties = QueueProperties::create().disable();
    Opencl opencl(queueProperties);
    Timer timer;
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemcpyINTEL = (pfn_clEnqueueMemcpyINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemcpyINTEL");
    if (!opencl.getExtensions().isUsmSupported()) {
        return TestResult::DriverFunctionNotFound;
    }

    // Create selected blitter queues
    struct PerQueueData {
        cl_command_queue queue; // freed by the OpenCL class
        std::string name;
        bool isLinkCopyEngine;
        cl_event event = nullptr;
        void *copySrc;
        void *copyDst;
        size_t copySize = 0;
    };
    size_t mainCopyEngineCount = 0;
    size_t linkCopyEngineCount = 0;
    std::vector<PerQueueData> queues;
    for (size_t blitterIndex : arguments.blitters.getEnabledBits()) {
        const bool isLinkCopyEngine = blitterIndex > 0;
        if (isLinkCopyEngine) {
            linkCopyEngineCount++;
        } else {
            mainCopyEngineCount++;
        }

        const Engine engine = EngineHelper::getBlitterEngineFromIndex(blitterIndex);
        const QueueProperties blitterQueueProperties = QueueProperties::create().setForceEngine(engine).setProfiling(true);
        const cl_command_queue queue = opencl.createQueue(blitterQueueProperties);
        if (queue == nullptr) {
            return TestResult::DeviceNotCapable;
        }
        const std::string queueName = EngineHelper::getEngineName(engine);
        queues.push_back(PerQueueData{queue, queueName, isLinkCopyEngine});
    }

    // Create buffers
    void *srcBuffer = UsmHelper::allocate(arguments.sourcePlacement, opencl.platform, opencl.context, opencl.device, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    void *dstBuffer = UsmHelper::allocate(arguments.destinationPlacement, opencl.platform, opencl.context, opencl.device, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Calculate copyOffset and copySize for each copy engine
    const size_t chunksCount = linkCopyEngineCount + mainCopyEngineCount * linkCopyEngineCount;
    const size_t chunksSize = arguments.size / chunksCount;
    const size_t copySizeForMainCopyEngine = chunksSize * linkCopyEngineCount;
    const size_t copySizeForLinkCopyEngine = chunksSize;
    size_t copyOffset = 0;
    for (auto i = 0u; i < queues.size(); i++) {
        const size_t copySize = queues[i].isLinkCopyEngine ? copySizeForLinkCopyEngine : copySizeForMainCopyEngine;
        queues[i].copySrc = static_cast<char *>(srcBuffer) + copyOffset;
        queues[i].copyDst = static_cast<char *>(dstBuffer) + copyOffset;
        queues[i].copySize = copySize;
        copyOffset += copySize;
    }
    DEVELOPER_WARNING_IF(copyOffset < arguments.size, "Buffer is divided to ", chunksCount, " chunks, but bufferSize is ",
                         "not divisible by ", chunksCount, ". Only ", copyOffset, " out of specified ", arguments.size, " bytes will be copied.");
    DEVELOPER_WARNING_IF(copyOffset > arguments.size, "Access out of buffer bounds will happen");

    // Warmup
    for (PerQueueData &queue : queues) {
        ASSERT_CL_SUCCESS(clEnqueueMemcpyINTEL(queue.queue, CL_FALSE, queue.copyDst, queue.copySrc, queue.copySize, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFlush(queue.queue));
    }
    for (PerQueueData &queue : queues) {
        ASSERT_CL_SUCCESS(clFinish(queue.queue));
    }

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        for (PerQueueData &queue : queues) {
            ASSERT_CL_SUCCESS(clEnqueueMemcpyINTEL(queue.queue, CL_FALSE, queue.copyDst, queue.copySrc, queue.copySize, 0, nullptr, &queue.event));
        }
        for (PerQueueData &queue : queues) {
            ASSERT_CL_SUCCESS(clFlush(queue.queue));
        }
        for (PerQueueData &queue : queues) {
            ASSERT_CL_SUCCESS(clFinish(queue.queue));
        }
        timer.measureEnd();

        // Report individual engines results and get time of the slowest engine
        std::chrono::nanoseconds maxGpuTime{};
        for (PerQueueData &queue : queues) {
            cl_ulong timeNs = 0ul;
            ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(queue.event, timeNs));
            ASSERT_CL_SUCCESS(clReleaseEvent(queue.event));
            statistics.pushValue(std::chrono::nanoseconds(timeNs), queue.copySize, queue.name);

            maxGpuTime = std::max(maxGpuTime, std::chrono::nanoseconds(timeNs));
        }

        // Report total results
        statistics.pushValue(maxGpuTime, arguments.size, "Total (Gpu)");
        statistics.pushValue(timer.get(), arguments.size, "Total (Cpu)");
    }

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, srcBuffer));
    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, dstBuffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmCopyMultipleBlits> registerTestCase(run, Api::OpenCL, true);
