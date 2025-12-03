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

#include "definitions/kernel_with_work.h"

#include <gtest/gtest.h>

static TestResult run(const KernelWithWorkArguments &arguments, Statistics &statistics) {
    MeasurementFields typeSelector(MeasurementUnit::Microseconds, MeasurementType::Cpu);

    if (isNoopRun()) {
        statistics.pushUnitAndType(typeSelector.getUnit(), typeSelector.getType());
        return TestResult::Nooped;
    }

    // Setup
    Cuda cuda;
    Timer timer;

    // Create output buffer
    const auto bufferSize = sizeof(uint32_t) * arguments.workgroupCount * arguments.workgroupSize * 2;
    void *outputBuffer = cuda.memAlloc(bufferSize);

    // Create kernel
    dim3 blockSize(static_cast<unsigned int>(arguments.workgroupSize), 1, 1);
    dim3 gridSize(static_cast<unsigned int>(arguments.workgroupCount), 1, 1);

    void *kernelFunction = cuda.loadKernel(selectKernel(arguments.usedIds, "fatbin"), "write_one");
    void *kernelArgs[] = {&outputBuffer};

    // Warmup
    cuda.launchKernel(kernelFunction, gridSize, blockSize, kernelArgs);
    cuda.synchronize();

    // Benchmark
    for (auto i = 0u; i < arguments.iterations; i++) {
        if (arguments.usedIds == WorkItemIdUsage::AtomicPerWorkgroup) {
            uint32_t workgroupCount = static_cast<uint32_t>(arguments.workgroupCount);
            cuda.memcpy(outputBuffer, &workgroupCount, sizeof(uint32_t), MemcpyDirection::HostToDevice);
        }
        timer.measureStart();
        cuda.launchKernel(kernelFunction, gridSize, blockSize, kernelArgs);
        cuda.synchronize();
        timer.measureEnd();
        statistics.pushValue(timer.get(), typeSelector.getUnit(), typeSelector.getType());
        if (arguments.usedIds == WorkItemIdUsage::AtomicPerWorkgroup) {
            uint32_t returnedValue[2] = {0u, 0u};
            cuda.memcpy(returnedValue, outputBuffer, sizeof(uint32_t) * 2, MemcpyDirection::DeviceToHost);
            EXPECT_EQ(0u, returnedValue[0]);
            EXPECT_EQ(1337u, returnedValue[1]);
        }
    }

    // Cleanup
    cuda.memFree(outputBuffer);

    return TestResult::Success;
}

static RegisterTestCaseImplementation<KernelWithWork> registerTestCase(run, Api::OPT);
