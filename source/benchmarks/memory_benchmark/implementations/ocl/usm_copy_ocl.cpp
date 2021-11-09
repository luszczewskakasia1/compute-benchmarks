/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#include "framework/ocl/opencl.h"
#include "framework/ocl/utility/buffer_contents_helper_ocl.h"
#include "framework/ocl/utility/profiling_helper.h"
#include "framework/ocl/utility/usm_helper_ocl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/usm_copy.h"

#include <gtest/gtest.h>

static TestResult run(const UsmCopyArguments &arguments, Statistics &statistics) {
    if (arguments.reuseCommandList == true ||
        arguments.sourcePlacement == UsmMemoryPlacement::NonUsmImported ||
        arguments.destinationPlacement == UsmMemoryPlacement::NonUsmImported) {
        return TestResult::ApiNotCapable;
    }

    // Setup
    QueueProperties queueProperties = QueueProperties::create().setProfiling(arguments.useEvents).setForceBlitter(arguments.forceBlitter).allowCreationFail();
    Opencl opencl(queueProperties);
    if (opencl.commandQueue == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    if (!QueueFamiliesHelper::validateCapabilities(opencl.commandQueue, CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL, CL_QUEUE_CAPABILITY_FILL_BUFFER_INTEL)) {
        return TestResult::DeviceNotCapable;
    }
    Timer timer;
    auto clEnqueueMemcpyINTEL = (pfn_clEnqueueMemcpyINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemcpyINTEL");
    if (!opencl.getExtensions().isUsmSupported()) {
        return TestResult::DriverFunctionNotFound;
    }

    // Create buffers
    UsmHelperOcl::Alloc srcAlloc{};
    UsmHelperOcl::Alloc dstAlloc{};
    ASSERT_CL_SUCCESS(UsmHelperOcl::allocate(opencl, arguments.sourcePlacement, arguments.size, srcAlloc));
    ASSERT_CL_SUCCESS(UsmHelperOcl::allocate(opencl, arguments.destinationPlacement, arguments.size, dstAlloc));

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueMemcpyINTEL(opencl.commandQueue, CL_FALSE, dstAlloc.ptr, srcAlloc.ptr, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (auto i = 0u; i < arguments.iterations; i++) {
        if (arguments.sourcePlacement == UsmMemoryPlacement::NonUsm) {
            BufferContentsHelperOcl::fillWithZeros(srcAlloc.ptr, arguments.size);
        } else {
            ASSERT_CL_SUCCESS(BufferContentsHelperOcl::fillUsmBuffer(opencl.commandQueue, srcAlloc.ptr, arguments.size, arguments.contents));
        }
        if (arguments.destinationPlacement == UsmMemoryPlacement::NonUsm) {
            BufferContentsHelperOcl::fillWithZeros(dstAlloc.ptr, arguments.size);
        } else {
            ASSERT_CL_SUCCESS(BufferContentsHelperOcl::fillUsmBuffer(opencl.commandQueue, dstAlloc.ptr, arguments.size, arguments.contents));
        }

        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueMemcpyINTEL(opencl.commandQueue, CL_FALSE, dstAlloc.ptr, srcAlloc.ptr, arguments.size, 0, nullptr, eventForEnqueue));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();

        if (eventForEnqueue) {
            cl_ulong timeNs{};
            ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(profilingEvent, timeNs));
            ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
            statistics.pushValue(std::chrono::nanoseconds(timeNs), arguments.size, MeasurementUnit::GigabytesPerSecond, MeasurementType::Gpu);
        } else {
            statistics.pushValue(timer.get(), arguments.size, MeasurementUnit::GigabytesPerSecond, MeasurementType::Cpu);
        }
    }

    ASSERT_CL_SUCCESS(UsmHelperOcl::deallocate(srcAlloc));
    ASSERT_CL_SUCCESS(UsmHelperOcl::deallocate(dstAlloc));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmCopy> registerTestCase(run, Api::OpenCL, true);
