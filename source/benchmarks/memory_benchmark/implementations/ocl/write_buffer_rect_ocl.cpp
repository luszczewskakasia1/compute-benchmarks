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
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/write_buffer_rect.h"

#include <gtest/gtest.h>

static TestResult run(const WriteBufferRectArguments &arguments, Statistics &statistics) {
    if (arguments.compressed && arguments.noIntelExtensions) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    Opencl opencl;
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

    // Fill the buffer
    const char pattern[] = {0};
    ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, buffer, pattern, sizeof(pattern) / sizeof(pattern[0]), 0, arguments.size, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Warmup
    size_t bufferOffset[3] = {};
    ASSERT_CL_SUCCESS(clEnqueueWriteBufferRect(opencl.commandQueue, buffer, CL_BLOCKING,
                                               bufferOffset, arguments.origin, arguments.region,
                                               arguments.rPitch, arguments.sPitch, arguments.rPitch, arguments.sPitch,
                                               cpuBuffer.get(), 0, nullptr, nullptr));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueWriteBufferRect(opencl.commandQueue, buffer, CL_NON_BLOCKING,
                                                   bufferOffset, arguments.origin, arguments.region,
                                                   arguments.rPitch, arguments.sPitch, arguments.rPitch, arguments.sPitch,
                                                   cpuBuffer.get(), 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))
        timer.measureEnd();
        statistics.pushValue(timer.get(), arguments.size, MeasurementUnit::GigabytesPerSecond, MeasurementType::Cpu);
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<WriteBufferRect> registerTestCase(run, Api::OpenCL);
