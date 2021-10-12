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

#include "definitions/flush_time.h"

#include <gtest/gtest.h>

static TestResult run(const FlushTimeArguments &arguments, Statistics &statistics) {
    // Setup
    QueueProperties queueProperties = QueueProperties::create().setOoq(arguments.useOoq);
    Opencl opencl(queueProperties);
    Timer timer;
    cl_int retVal;

    // Get parameters for the enqueue call
    cl_event event{};
    cl_event *eventForNdr = arguments.useEvent ? &event : nullptr;
    size_t gws = arguments.workgroupCount * arguments.workgroupSize;
    const size_t lws = arguments.workgroupSize;
    const size_t *lwsForNdr = (lws != 0) ? &lws : nullptr;

    if (gws == 0) {
        gws = 1;
    }

    // Create kernel
    const char *source = "__kernel void empty() {}";
    const auto sourceLength = strlen(source);
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "empty", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup, kernel
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, lwsForNdr, 0, nullptr, eventForNdr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
    if (eventForNdr) {
        ASSERT_CL_SUCCESS(clReleaseEvent(event));
    }
    ASSERT_CL_SUCCESS(retVal);

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, lwsForNdr, 0, nullptr, eventForNdr));
        timer.measureStart();
        ASSERT_CL_SUCCESS(clFlush(opencl.commandQueue));
        timer.measureEnd();
        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);
        if (eventForNdr) {
            ASSERT_CL_SUCCESS(clReleaseEvent(event));
        }
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<FlushTime> registerTestCase(run, Api::OpenCL);
