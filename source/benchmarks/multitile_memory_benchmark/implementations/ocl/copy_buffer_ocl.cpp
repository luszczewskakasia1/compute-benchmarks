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
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/copy_buffer.h"

#include <gtest/gtest.h>

static TestResult run(const CopyBufferArguments &arguments, Statistics &statistics) {
    if ((arguments.dstCompressed || arguments.srcCompressed) && arguments.noIntelExtensions) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    cl_int retVal;
    QueueProperties queueProperties = QueueProperties::create().setDeviceSelection(arguments.queuePlacement).setProfiling(arguments.useEvents);
    ContextProperties contextProperties = ContextProperties::create().setDeviceSelection(arguments.contextPlacement).allowCreationFail();
    Opencl opencl(queueProperties, contextProperties);
    if (opencl.context == nullptr) {
        return TestResult::DeviceNotCapable;
    }
    auto clCreateBufferWithPropertiesINTEL = (pfn_clCreateBufferWithPropertiesINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clCreateBufferWithPropertiesINTEL");
    if (!clCreateBufferWithPropertiesINTEL) {
        return TestResult::DriverFunctionNotFound;
    }
    Timer timer;

    // Create buffers
    const cl_mem_properties_intel memPropertiesSrc[] = {
        CL_MEM_FLAGS,
        CL_MEM_READ_WRITE | CompressionHelper::getCompressionFlags(arguments.srcCompressed, arguments.noIntelExtensions),
        CL_MEM_DEVICE_ID_INTEL,
        (cl_mem_properties_intel)opencl.getDevice(arguments.srcPlacement),
        0,
    };
    const cl_mem_properties_intel memPropertiesDst[] = {
        CL_MEM_FLAGS,
        CL_MEM_READ_WRITE | CompressionHelper::getCompressionFlags(arguments.dstCompressed, arguments.noIntelExtensions),
        CL_MEM_DEVICE_ID_INTEL,
        (cl_mem_properties_intel)opencl.getDevice(arguments.dstPlacement),
        0,
    };
    const cl_mem source = clCreateBufferWithPropertiesINTEL(opencl.context, memPropertiesSrc, 0, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    const cl_mem destination = clCreateBufferWithPropertiesINTEL(opencl.context, memPropertiesDst, 0, arguments.size, nullptr, &retVal);
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
            statistics.pushValue(std::chrono::nanoseconds(timeNs), arguments.size, MeasurementUnit::GigabytesPerSecond, MeasurementType::Gpu);
        } else {
            statistics.pushValue(timer.get(), arguments.size, MeasurementUnit::GigabytesPerSecond, MeasurementType::Cpu);
        }
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(destination));
    ASSERT_CL_SUCCESS(clReleaseMemObject(source));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<CopyBuffer> registerTestCase(run, Api::OpenCL, true);
