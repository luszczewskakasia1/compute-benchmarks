#include "framework/ocl/opencl.h"
#include "framework/ocl/utility/profiling_helper.h"
#include "framework/ocl/utility/usm_helper.h"
#include "framework/test_case/register_test_case.h"

#include "definitions/simultaneous_blits.h"

#include <gtest/gtest.h>

static TestResult run(const SimultaneousBlitterCopiesArguments &arguments, Statistics &statistics) {
    // Setup
    cl_int retVal{};
    QueueProperties queueProperties = QueueProperties::create().disable();
    Opencl opencl(queueProperties);
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemcpyINTEL = (pfn_clEnqueueMemcpyINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemcpyINTEL");
    if (!opencl.getExtensions().isUsmSupported()) {
        return TestResult::DriverFunctionNotFound;
    }

    // Create selected blitter queues
    struct PerQueueData {
        cl_command_queue queue = nullptr; // freed by the OpenCL class
        std::string name = "";
        void *srcBuffer = nullptr;
        void *dstBuffer = nullptr;
        cl_event event = nullptr;
    };
    std::vector<PerQueueData> queues;
    for (size_t blitterIndex : arguments.blitters.getEnabledBits()) {
        const Engine engine = EngineHelper::getBlitterEngineFromIndex(blitterIndex);
        const QueueProperties blitterQueueProperties = QueueProperties::create().setForceEngine(engine).setProfiling(true);
        const cl_command_queue queue = opencl.createQueue(blitterQueueProperties);
        if (queue == nullptr) {
            return TestResult::DeviceNotCapable;
        }
        const std::string queueName = EngineHelper::getEngineName(engine);
        queues.push_back(PerQueueData{queue, queueName});
    }

    // Create buffers
    for (PerQueueData &queue : queues) {
        queue.srcBuffer = UsmHelper::allocate(arguments.sourcePlacement, opencl.platform, opencl.context, opencl.device, arguments.size, &retVal);
        ASSERT_CL_SUCCESS(retVal);
        queue.dstBuffer = UsmHelper::allocate(arguments.destinationPlacement, opencl.platform, opencl.context, opencl.device, arguments.size, &retVal);
        ASSERT_CL_SUCCESS(retVal);
    }

    // Warmup
    for (PerQueueData &queue : queues) {
        ASSERT_CL_SUCCESS(clEnqueueMemcpyINTEL(queue.queue, CL_FALSE, queue.dstBuffer, queue.srcBuffer, arguments.size, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFlush(queue.queue));
    }
    for (PerQueueData &queue : queues) {
        ASSERT_CL_SUCCESS(clFinish(queue.queue));
    }

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        for (PerQueueData &queue : queues) {
            ASSERT_CL_SUCCESS(clEnqueueMemcpyINTEL(queue.queue, CL_FALSE, queue.dstBuffer, queue.srcBuffer, arguments.size, 0, nullptr, &queue.event));
        }
        for (PerQueueData &queue : queues) {
            ASSERT_CL_SUCCESS(clFlush(queue.queue));
        }

        uint64_t totalSize = arguments.size * queues.size();
        std::chrono::nanoseconds totalTime{};
        for (PerQueueData &queue : queues) {
            cl_ulong timeNs = 0ul;
            ASSERT_CL_SUCCESS(clFinish(queue.queue));
            ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(queue.event, timeNs));
            ASSERT_CL_SUCCESS(clReleaseEvent(queue.event));

            totalTime += std::chrono::nanoseconds(timeNs);
            statistics.pushValue(std::chrono::nanoseconds(timeNs), arguments.size, queue.name);
        }

        statistics.pushValue(totalTime, totalSize);
    }

    for (PerQueueData &queue : queues) {
        ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, queue.srcBuffer));
        ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, queue.dstBuffer));
    }
    return TestResult::Success;
}

static RegisterTestCaseImplementation<SimultaneousBlits> registerTestCase(run, Api::OpenCL, true);
