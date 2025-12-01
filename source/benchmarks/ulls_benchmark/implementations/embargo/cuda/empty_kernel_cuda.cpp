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

#include "framework/additional/embargo/additional_configuration.h"
#include "framework/embargo/cuda/cuda.h"
#include "framework/embargo/cuda/utility/error.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/empty_kernel.h"

#include <gtest/gtest.h>

static TestResult run(const EmptyKernelArguments &arguments, Statistics &statistics) {
    MeasurementFields typeSelector(MeasurementUnit::Microseconds, MeasurementType::Cpu);

    if (isNoopRun()) {
        statistics.pushUnitAndType(typeSelector.getUnit(), typeSelector.getType());
        return TestResult::Nooped;
    }

    // Setup
    Cuda cuda(Configuration::get().additionalConfiguration.cudaApiType);
    Timer timer;

    // Create kernel
    dim3 blockSize(static_cast<unsigned int>(arguments.workgroupSize), 1, 1);
    dim3 gridSize(static_cast<unsigned int>(arguments.workgroupCount), 1, 1);

    void *kernelFunction = cuda.loadKernel("ulls_benchmark_empty_kernel.fatbin", "emptyKernel");

    // Warmup
    cuda.launchKernel(kernelFunction, gridSize, blockSize, nullptr);
    cuda.synchronize();

    // Benchmark
    for (auto i = 0u; i < arguments.iterations; i++) {
        timer.measureStart();

        cuda.launchKernel(kernelFunction, gridSize, blockSize, nullptr);
        cuda.synchronize();

        timer.measureEnd();
        statistics.pushValue(timer.get(), typeSelector.getUnit(), typeSelector.getType());
    }

    return TestResult::Success;
}

static RegisterTestCaseImplementation<EmptyKernel> registerTestCase(run, Api::OPT);
