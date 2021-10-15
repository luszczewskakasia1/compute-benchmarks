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

#include "definitions/copy_buffer_rect.h"

#include <gtest/gtest.h>

static TestResult run(const CopyBufferRectArguments &arguments, Statistics &statistics) {
    if ((arguments.compressedDestination || arguments.compressedSource) && arguments.noIntelExtensions) {
        return TestResult::DeviceNotCapable;
    }

    // Setup
    Opencl opencl;
    Timer timer;
    cl_int retVal;

    // Create buffers
    const cl_mem_flags compressionHintSrc = CompressionHelper::getCompressionFlags(arguments.compressedSource, arguments.noIntelExtensions);
    const cl_mem_flags compressionHintDst = CompressionHelper::getCompressionFlags(arguments.compressedDestination, arguments.noIntelExtensions);
    const cl_mem sourceBuffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE | compressionHintSrc, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    const cl_mem destinationBuffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE | compressionHintDst, arguments.size, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Check buffers compression
    const auto srcCompressionStatus = CompressionHelper::verifyCompression(sourceBuffer, arguments.compressedSource, arguments.noIntelExtensions);
    const auto dstCompressionStatus = CompressionHelper::verifyCompression(destinationBuffer, arguments.compressedDestination, arguments.noIntelExtensions);
    if (srcCompressionStatus != TestResult::Success || dstCompressionStatus != TestResult::Success) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(sourceBuffer));
        ASSERT_CL_SUCCESS(clReleaseMemObject(destinationBuffer));
        if (srcCompressionStatus != TestResult::Success) {
            return srcCompressionStatus;
        } else {
            return dstCompressionStatus;
        }
    }

    // Warmup
    ASSERT_CL_SUCCESS(clEnqueueCopyBufferRect(opencl.commandQueue, sourceBuffer, destinationBuffer,
                                              arguments.origin, arguments.origin, arguments.region,
                                              arguments.rPitch, arguments.sPitch, arguments.rPitch, arguments.sPitch,
                                              0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueCopyBufferRect(opencl.commandQueue, sourceBuffer, destinationBuffer,
                                                  arguments.origin, arguments.origin, arguments.region,
                                                  arguments.rPitch, arguments.sPitch, arguments.rPitch, arguments.sPitch,
                                                  0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue))
        timer.measureEnd();
        statistics.pushValue(timer.get(), arguments.size, MeasurementUnit::GigabytesPerSecond, MeasurementType::Cpu);
    }

    ASSERT_CL_SUCCESS(clReleaseMemObject(sourceBuffer));
    ASSERT_CL_SUCCESS(clReleaseMemObject(destinationBuffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<CopyBufferRect> registerTestCase(run, Api::OpenCL);
