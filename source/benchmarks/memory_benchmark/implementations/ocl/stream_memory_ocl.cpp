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
#include "framework/ocl/utility/program_helper_ocl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/compiler_options_builder.h"
#include "framework/utility/timer.h"

#include "definitions/stream_memory.h"

#include <gtest/gtest.h>

static TestResult run(const StreamMemoryArguments &arguments, Statistics &statistics) {
    // Setup
    cl_int retVal = {};
    QueueProperties queueProperties = QueueProperties::create().setProfiling(true).setOoq(0);
    Opencl opencl(queueProperties);
    Timer timer;
    const size_t elementSize = 4u; // TODO this is dependent on datatype
    const size_t fillValue = 313u;
    const int32_t scalarValue = -999;

    // Create kernel-specific buffers
    const char *kernelName = {};
    cl_mem buffers[3] = {};
    size_t buffersCount = {};
    switch (arguments.type) {
    case StreamMemoryType::Read:
        kernelName = "read";
        buffers[buffersCount++] = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, arguments.size, nullptr, &retVal);
        buffers[buffersCount++] = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, 16u, nullptr, &retVal);
        break;
    case StreamMemoryType::Write:
        kernelName = "write";
        buffers[buffersCount++] = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, arguments.size, nullptr, &retVal);
        break;
    case StreamMemoryType::Scale:
        kernelName = "scale";
        buffers[buffersCount++] = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, arguments.size, nullptr, &retVal);
        buffers[buffersCount++] = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, arguments.size, nullptr, &retVal);
        break;
    case StreamMemoryType::Triad:
        kernelName = "triad";
        buffers[buffersCount++] = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, arguments.size, nullptr, &retVal);
        buffers[buffersCount++] = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, arguments.size, nullptr, &retVal);
        buffers[buffersCount++] = clCreateBuffer(opencl.context, CL_MEM_READ_WRITE, arguments.size, nullptr, &retVal);
        break;
    default:
        FATAL_ERROR("Unknown StreamMemoryType");
    }

    // Create kernel
    CompilerOptionsBuilder compilerOptions;
    compilerOptions.addDefinitionKeyValue("STREAM_TYPE", "float");
    const char *programName = "memory_benchmark_stream_memory.cl";
    cl_program program{};
    if (auto result = ProgramHelperOcl::buildProgramFromSourceFile(opencl.context, opencl.device, programName, compilerOptions.str().c_str(), program); result != TestResult::Success) {
        return result;
    }
    cl_kernel kernel = clCreateKernel(program, kernelName, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    for (auto i = 0u; i < buffersCount; i++) {
        ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, buffers[i], &fillValue, sizeof(fillValue), 0, arguments.size, 0, nullptr, nullptr));
        ASSERT_CL_SUCCESS(clSetKernelArg(kernel, static_cast<cl_uint>(i), sizeof(buffers[i]), &buffers[i]))
    }
    ASSERT_CL_SUCCESS(clSetKernelArg(kernel, static_cast<cl_uint>(buffersCount), sizeof(scalarValue), &scalarValue));

    // Warmup
    const size_t gws = arguments.size / elementSize;
    ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, nullptr));
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));

    // Benchmark
    for (auto i = 0u; i < arguments.iterations; i++) {
        cl_event profilingEvent{};
        cl_event *eventForEnqueue = arguments.useEvents ? &profilingEvent : nullptr;

        for (size_t j = 0; j < buffersCount; j++) {
            ASSERT_CL_SUCCESS(clEnqueueFillBuffer(opencl.commandQueue, buffers[j], &fillValue, sizeof(fillValue), 0, arguments.size, 0, nullptr, nullptr));
        }

        timer.measureStart();
        ASSERT_CL_SUCCESS(clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, eventForEnqueue));
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

    // Cleanup
    for (size_t i = 0; i < buffersCount; i++) {
        ASSERT_CL_SUCCESS(clReleaseMemObject(buffers[i]));
    }
    ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
    ASSERT_CL_SUCCESS(clReleaseProgram(program));
    return TestResult::Success;
}

static RegisterTestCaseImplementation<StreamMemory> registerTestCase(run, Api::OpenCL);
