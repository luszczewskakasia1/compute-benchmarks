/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2025-2026 Intel Corporation
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

#include "framework/embargo/cuda/cuda.h"
#include "framework/embargo/cuda/utility/error.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/multi_kernel_execution.h"

#include <gtest/gtest.h>

static TestResult run(const MultiKernelExecutionArguments &arguments, Statistics &statistics) {
    MeasurementFields typeSelector(MeasurementUnit::Microseconds, arguments.profiling ? MeasurementType::Gpu : MeasurementType::Cpu);

    if (arguments.inOrderOverOOO || arguments.useMCL) {
        return TestResult::DeviceNotCapable;
    }

    if (isNoopRun()) {
        statistics.pushUnitAndType(typeSelector.getUnit(), typeSelector.getType());
        return TestResult::Nooped;
    }

    // Setup
    Cuda cuda;
    Timer timer;

    // Create kernel
    dim3 blockSize(static_cast<unsigned int>(arguments.workgroupSize), 1, 1);
    dim3 gridSize(static_cast<unsigned int>(arguments.workgroupCount), 1, 1);

    void *kernelFunction = cuda.loadKernel("ulls_benchmark_multi_kernel_execution.fatbin", "emptyKernel");

    CUstream_st *stream = cuda.streamCreate();
    CUevent_st *start = nullptr;
    CUevent_st *stop = nullptr;

    if (arguments.profiling) {
        start = cuda.createEvent();
        stop = cuda.createEvent();
    }

    cuda.streamBeginCapture(stream);
    if (arguments.profiling) {
        cuda.eventRecordWithFlags(start, stream, true);
    }
    for (auto i = 0u; i < arguments.kernelCount; i++) {
        cuda.launchKernel(kernelFunction, gridSize, blockSize, stream, nullptr);
    }
    if (arguments.profiling) {
        cuda.eventRecordWithFlags(stop, stream, true);
    }
    CUgraph_st *graph = cuda.streamEndCapture(stream);
    CUgraphExec_st *graphExec = cuda.graphInstantiate(graph);

    // Warmup
    cuda.graphLaunch(graphExec, stream);
    cuda.streamSynchronize(stream);

    // Benchmark
    for (auto iteration = 0u; iteration < arguments.iterations; iteration++) {
        timer.measureStart();
        cuda.graphLaunch(graphExec, stream);
        if (arguments.profiling) {
            cuda.eventSynchronize(stop);
        } else {
            cuda.streamSynchronize(stream);
        }
        timer.measureEnd();

        if (arguments.profiling) {
            auto timeNs = cuda.getEventDurationInNanoseconds(start, stop);
            statistics.pushValue(std::chrono::nanoseconds(timeNs), typeSelector.getUnit(), typeSelector.getType());
        } else {
            statistics.pushValue(timer.get(), typeSelector.getUnit(), typeSelector.getType());
        }
    }

    // Cleanup
    if (arguments.profiling) {
        cuda.eventDestroy(stop);
        cuda.eventDestroy(start);
    }
    cuda.graphExecDestroy(graphExec);
    cuda.graphDestroy(graph);
    cuda.streamDestroy(stream);

    return TestResult::Success;
}

static RegisterTestCaseImplementation<MultiKernelExecution> registerTestCase(run, Api::OPT);
