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
#include "framework/ocl/utility/usm_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/usm_shared_first_cpu_access.h"

#include <gtest/gtest.h>

static TestResult run(const UsmSharedFirstCpuAccessArguments &arguments, Statistics &statistics) {
    // Setup
    Opencl opencl;
    Timer timer;
    auto clSharedMemAllocINTEL = (pfn_clSharedMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clSharedMemAllocINTEL");
    auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clMemFreeINTEL");
    if (!clSharedMemAllocINTEL || !clMemFreeINTEL) {
        return TestResult::DriverFunctionNotFound;
    }
    cl_int retVal{};

    // Warmup
    const cl_mem_properties_intel properties[] = {
        CL_MEM_ALLOC_FLAGS_INTEL,
        UsmHelper::getInitialPlacementFlag(arguments.initialPlacement),
        0,
    };
    auto buffer = clSharedMemAllocINTEL(opencl.context, opencl.device, properties, arguments.bufferSize, 0u, &retVal);
    ASSERT_CL_SUCCESS(retVal);
    static_cast<uint8_t *>(buffer)[0] = 0;
    ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));

    // Benchmark
    for (int i = 0; i < arguments.iterations; i++) {
        buffer = clSharedMemAllocINTEL(opencl.context, opencl.device, properties, arguments.bufferSize, 0u, &retVal);
        ASSERT_CL_SUCCESS(retVal);

        timer.measureStart();
        static_cast<uint32_t *>(buffer)[0] = 0;
        timer.measureEnd();

        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);
        ASSERT_CL_SUCCESS(clMemFreeINTEL(opencl.context, buffer));
    }

    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmSharedFirstCpuAccess> registerTestCase(run, Api::OpenCL, true);
