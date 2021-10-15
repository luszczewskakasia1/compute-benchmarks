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
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/new_resources_with_gpu_access.h"

#include <gtest/gtest.h>

static TestResult run(const NewResourcesWithGpuAccessArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    cl_int retVal;

    // Create kernel
    const char *source = "__kernel void write(__global int *outBuffer) {  \n"
                         "   outBuffer[get_global_id(0)] = 1;             \n"
                         "}";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "write", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Get work size
    const size_t elements = arguments.size / sizeof(cl_uint);
    size_t lws{};
    ASSERT_CL_SUCCESS(clGetKernelWorkGroupInfo(kernel, opencl.device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(lws), &lws, nullptr));
    size_t wgc = elements / lws; // may be rounded down, but that shouldn't matter for big buffers
    if (wgc == 0) {
        // very small buffer
        lws = elements;
        wgc = 1;
    }
    size_t gws = wgc * lws;

    // Warmup kernel
    const size_t sizeInBytes = arguments.size;
    cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, sizeInBytes, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(buffer), &buffer));
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    cl_mem previousBuffer = buffer;
    for (auto i = 0u; i < arguments.iterations; i++) {
        timer.measureStart();
        buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, sizeInBytes, nullptr, &retVal);
        ASSERT_CL_SUCCESS(retVal);
        ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(buffer), &buffer));
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();
        ASSERT_CL_SUCCESS(retVal);
        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);

        // Store buffer used in this iteration to avoid reuse
        ASSERT_CL_SUCCESS(clReleaseMemObject(previousBuffer));
        previousBuffer = buffer;
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseMemObject(previousBuffer));
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<NewResourcesWithGpuAccess> registerTestCase(run, Api::OpenCL);
