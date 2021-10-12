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
#include "framework/ocl/utility/usm_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/usm_fill_specific_pattern.h"

#include <gtest/gtest.h>

static TestResult run(const UsmFillSpecificPatternArguments &arguments, Statistics &statistics) {
    // Setup
    cl_int retVal{};
    QueueProperties queueProperties = QueueProperties::create().setProfiling(arguments.useEvents).setForceBlitter(arguments.forceBlitter).allowCreationFail();
    Opencl opencl(queueProperties);
    if (opencl.commandQueue == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    if (!QueueFamiliesHelper::validateCapability(opencl.commandQueue, CL_QUEUE_CAPABILITY_FILL_BUFFER_INTEL)) {
        return TestResult::DeviceNotCapable;
    }
    Timer timer;
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemFillINTEL = (pfn_clEnqueueMemFillINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemFillINTEL");
    if (!opencl.getExtensions().isUsmSupported()) {
        return TestResult::DriverFunctionNotFound;
    }

    // Create buffer
    void *buffer = UsmHelper::allocate(arguments.usmMemoryPlacement, opencl.platform, opencl.context, opencl.device, arguments.bufferSize, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup
    const std::vector<uint8_t> &pattern = arguments.pattern;
    ASSERT_CL_SUCCESS(clEnqueueMemFillINTEL(opencl.commandQueue, buffer, pattern.data(), pattern.size(), arguments.bufferSize, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        ASSERT_CL_SUCCESS(BufferContentsHelperOcl::fillUsmBuffer(opencl.commandQueue, buffer, arguments.bufferSize, arguments.contents))

        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueMemFillINTEL(opencl.commandQueue, buffer, pattern.data(), pattern.size(), arguments.bufferSize, 0, nullptr, eventForEnqueue));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();

        if (eventForEnqueue) {
            cl_ulong timeNs{};
            ASSERT_CL_SUCCESS(ProfilingHelper::getEventDurationInNanoseconds(profilingEvent, timeNs));
            ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
            statistics.pushValue(std::chrono::nanoseconds(timeNs), arguments.bufferSize, MeasurementUnit::GigabytesPerSecond, MeasurementType::Gpu);
        } else {
            statistics.pushValue(timer.get(), arguments.bufferSize, MeasurementUnit::GigabytesPerSecond, MeasurementType::Cpu);
        }
    }

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmFillSpecificPattern> registerTestCase(run, Api::OpenCL, true);
