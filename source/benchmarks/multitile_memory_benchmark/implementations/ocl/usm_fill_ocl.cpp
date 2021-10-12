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
#include "framework/ocl/utility/compression_helper.h"
#include "framework/ocl/utility/profiling_helper.h"
#include "framework/ocl/utility/usm_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/usm_fill.h"

#include <gtest/gtest.h>

static TestResult run(const UsmFillArguments &arguments, Statistics &statistics) {
    // Setup
    cl_int retVal;
    QueueProperties queueProperties = QueueProperties::create().setDeviceSelection(arguments.queuePlacement).setForceBlitter(arguments.forceBlitter).setProfiling(arguments.useEvents).allowCreationFail();
    ContextProperties contextProperties = ContextProperties::create().setDeviceSelection(arguments.contextPlacement).allowCreationFail();
    Opencl opencl(queueProperties, contextProperties);
    if (opencl.context == nullptr || opencl.commandQueue == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    if (!QueueFamiliesHelper::validateCapability(opencl.commandQueue, CL_QUEUE_CAPABILITY_FILL_BUFFER_INTEL)) {
        return TestResult::DeviceNotCapable;
    }
    auto clCreateBufferWithPropertiesINTEL = (pfn_clCreateBufferWithPropertiesINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clCreateBufferWithPropertiesINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    auto clEnqueueMemFillINTEL = (pfn_clEnqueueMemFillINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clEnqueueMemFillINTEL");
    if (!clCreateBufferWithPropertiesINTEL || !opencl.getExtensions().isUsmSupported()) {
        return TestResult::DriverFunctionNotFound;
    }
    Timer timer;

    // Create buffer
    void *buffer = UsmHelper::allocate(arguments.bufferPlacement, opencl, arguments.size, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Create pattern
    const auto pattern = std::make_unique<uint8_t[]>(arguments.patternSize);

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueMemFillINTEL(opencl.commandQueue, buffer, pattern.get(), arguments.patternSize, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueMemFillINTEL(opencl.commandQueue, buffer, pattern.get(), arguments.patternSize, arguments.size, 0, nullptr, eventForEnqueue));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))
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

    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmFill> registerTestCase(run, Api::OpenCL, true);
