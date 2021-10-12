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
#include "framework/utility/file_helper.h"
#include "framework/utility/timer.h"

#include "definitions/kernel_with_work.h"

#include <gtest/gtest.h>

#define PROVIDE_PROFLING_DETAILS 0

static TestResult run(const KernelWithWorkArguments &arguments, Statistics &statistics) {
    // Setup
#if PROVIDE_PROFLING_DETAILS == 1
    QueueProperties queueProperties = QueueProperties::create().setProfiling(true);
    Opencl opencl(queueProperties);
#else
    Opencl opencl;
#endif
    Timer timer;
    cl_int retVal{};
    const size_t gws = arguments.workgroupCount * arguments.workgroupSize;
    const size_t lws = arguments.workgroupSize;

    // Create buffer
    const auto bufferSize = sizeof(cl_int) * arguments.workgroupCount * arguments.workgroupSize;
    cl_mem buffer = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, bufferSize, nullptr, &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Create kernel
    const std::vector<uint8_t> kernelSource = FileHelper::loadTextFile(selectKernel(arguments.usedIds, "cl"));
    if (kernelSource.size() == 0) {
        return TestResult::KernelNotFound;
    }
    const char *source = reinterpret_cast<const char *>(kernelSource.data());
    const size_t sourceLength = kernelSource.size();
    cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
    cl_kernel kernel = clCreateKernel(program, "write_one", &retVal);
    ASSERT_CL_SUCCESS(retVal);

    // Warmup, kernel
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, 0, sizeof(buffer), &buffer));
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
    ASSERT_CL_SUCCESS(retVal);

#if PROVIDE_PROFLING_DETAILS == 1
    cl_event profilingEvent;
    cl_ulong queued{}, start{}, end{};
#endif

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        timer.measureStart();
#if PROVIDE_PROFLING_DETAILS == 1
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, &profilingEvent));
#else
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, &lws, 0, nullptr, nullptr));
#endif

        ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
        timer.measureEnd();

#if PROVIDE_PROFLING_DETAILS == 1
        ASSERT_CL_SUCCESS(clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, nullptr));
        ASSERT_CL_SUCCESS(clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &queued, nullptr));
        ASSERT_CL_SUCCESS(clGetEventProfilingInfo(profilingEvent, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, nullptr));
        ASSERT_CL_SUCCESS(clReleaseEvent(profilingEvent));
        printf("\n Queued %lu Start %lu End %lu, delta %lu\n ", queued, start, end, end - start);
#endif
        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);
    }

    // Cleanup
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    ASSERT_CL_SUCCESS(clReleaseMemObject(buffer));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<KernelWithWork> registerTestCase(run, Api::OpenCL);
