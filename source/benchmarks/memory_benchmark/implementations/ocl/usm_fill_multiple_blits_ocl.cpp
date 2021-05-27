#include "framework/ocl/opencl.h"
#include "framework/ocl/utility/buffer_contents_helper_ocl.h"
#include "framework/ocl/utility/profiling_helper.h"
#include "framework/ocl/utility/usm_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "blit_size_assigner.h"
#include "definitions/usm_fill_multiple_blits.h"

#include <gtest/gtest.h>

static TestResult run(const UsmFillMultipleBlitsArguments &arguments, Statistics &statistics) {
    // Setup
    cl_int retVal{};
    QueueProperties queueProperties = QueueProperties::create().disable();
    Opencl opencl(queueProperties);
    Timer timer;
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemFillINTEL = (pfn_clEnqueueMemFillINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemFillINTEL");
    if (!opencl.getExtensions().isUsmSupported()) {
        return TestResult::DriverFunctionNotFound;
    }

    // Create selected blitter queues
    struct PerQueueData {
        cl_command_queue queue; // freed by the OpenCL class
        std::string name;
        bool isMainCopyEngine;
        cl_event event = nullptr;
        void *fillDst = nullptr;
        size_t fillSize = 0;
    };
    BlitSizeAssigner blitSizeAssigner{arguments.size};
    std::vector<PerQueueData> queues;
    for (size_t blitterIndex : arguments.blitters.getEnabledBits()) {
        const bool isMainCopyEngine = blitterIndex == 0;
        if (isMainCopyEngine) {
            blitSizeAssigner.addMainCopyEngine();
        } else {
            blitSizeAssigner.addLinkCopyEngine();
        }

        const Engine engine = EngineHelper::getBlitterEngineFromIndex(blitterIndex);
        const QueueProperties blitterQueueProperties = QueueProperties::create().setForceEngine(engine).setProfiling(true);
        const cl_command_queue queue = opencl.createQueue(blitterQueueProperties);
        if (queue == nullptr) {
            return TestResult::DeviceNotCapable;
        }
        if (!QueueFamiliesHelper::validateCapability(queue, CL_QUEUE_CAPABILITY_FILL_BUFFER_INTEL)) {
            return TestResult::DeviceNotCapable;
        }

        const std::string queueName = EngineHelper::getEngineName(engine);
        queues.push_back(PerQueueData{queue, queueName, isMainCopyEngine});
    }

    // Create buffer
    void *dstBuffer = UsmHelper::allocate(arguments.memoryPlacement, opencl.platform, opencl.context, opencl.device, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Calculate copyOffset and copySize for each copy engine
    for (auto i = 0u; i < queues.size(); i++) {
        const auto [offset, size] = blitSizeAssigner.getSpaceForBlit(queues[i].isMainCopyEngine);
        queues[i].fillDst = static_cast<char *>(dstBuffer) + offset;
        queues[i].fillSize = size;
    }
    blitSizeAssigner.validate();

    // Create pattern
    const auto pattern = std::make_unique<uint8_t[]>(arguments.patternSize);
    BufferContentsHelperOcl::fill(pattern.get(), arguments.patternSize, arguments.patternContents);

    // Warmup
    for (PerQueueData &queue : queues) {
        ASSERT_CL_SUCCESS(clEnqueueMemFillINTEL(queue.queue, queue.fillDst, pattern.get(), arguments.patternSize, queue.fillSize, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFlush(queue.queue));
    }
    for (PerQueueData &queue : queues) {
        ASSERT_CL_SUCCESS(clFinish(queue.queue));
    }

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        for (PerQueueData &queue : queues) {
            ASSERT_CL_SUCCESS(clEnqueueMemFillINTEL(queue.queue, queue.fillDst, pattern.get(), arguments.patternSize, queue.fillSize, 0, nullptr, nullptr));
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
            statistics.pushValue(std::chrono::nanoseconds(timeNs), arguments.size, queue.name);

            maxGpuTime = std::max(maxGpuTime, std::chrono::nanoseconds(timeNs));
        }

        // Report total results
        const uint64_t totalSize = arguments.size * queues.size();
        statistics.pushValue(maxGpuTime, totalSize, "Total (Gpu)");
        statistics.pushValue(timer.get(), totalSize, "Total (Cpu)");
    }

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, dstBuffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmFillMultipleBlits> registerTestCase(run, Api::OpenCL, true);
