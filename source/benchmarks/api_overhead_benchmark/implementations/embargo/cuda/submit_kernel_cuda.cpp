/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2025 Intel Corporation
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

#include "definitions/submit_kernel.h"

#include <gtest/gtest.h>

static TestResult run(const SubmitKernelArguments &arguments, Statistics &statistics) {
    MeasurementFields typeSelector(MeasurementUnit::Microseconds, MeasurementType::Cpu);

    if (!arguments.inOrderQueue) {
        return TestResult::ApiNotCapable;
    }

    if (isNoopRun()) {
        statistics.pushUnitAndType(typeSelector.getUnit(), typeSelector.getType());
        return TestResult::Nooped;
    }

    // Setup
    Cuda cuda;
    Timer timer;

    CUstream_st *stream = cuda.streamCreate();
    void *kernelFunction = cuda.loadKernel("api_overhead_benchmark_eat_time.fatbin", "eat_time");

    int kernelExecutionTime = static_cast<int>(arguments.kernelExecutionTime);
    dim3 blockSize(1, 1, 1);
    dim3 gridSize(1, 1, 1);

    void *kernelArgs[] = {&kernelExecutionTime};
    CUevent_st *event = nullptr;

    // Warmup
    for (auto iteration = 0u; iteration < arguments.numKernels; iteration++) {
        cuda.launchKernel(kernelFunction, gridSize, blockSize, stream, kernelArgs);

        if (arguments.useEvents) {
            event = cuda.createEvent(EventFlags{.disableTiming = !arguments.useProfiling});
            cuda.eventRecord(event, stream);
            cuda.eventDestroy(event);
        }
    }
    cuda.streamSynchronize(stream);

    // Benchmark
    for (auto i = 0u; i < arguments.iterations; i++) {
        timer.measureStart();

        for (auto iteration = 0u; iteration < arguments.numKernels; iteration++) {
            cuda.launchKernel(kernelFunction, gridSize, blockSize, stream, kernelArgs);

            if (arguments.useEvents) {
                event = cuda.createEvent(EventFlags{.disableTiming = !arguments.useProfiling});
                cuda.eventRecord(event, stream);
                cuda.eventDestroy(event);
            }
        }

        if (!arguments.measureCompletionTime) {
            timer.measureEnd();
            statistics.pushValue(timer.get(), typeSelector.getUnit(), typeSelector.getType());
        }

        cuda.streamSynchronize(stream);

        if (arguments.measureCompletionTime) {
            timer.measureEnd();
            statistics.pushValue(timer.get(), typeSelector.getUnit(), typeSelector.getType());
        }
    }

    // Cleanup
    cuda.streamDestroy(stream);

    return TestResult::Success;
}

[[maybe_unused]] static RegisterTestCaseImplementation<SubmitKernel> registerTestCase(run, Api::OPT);
