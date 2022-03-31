/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2022 Intel Corporation
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

#include "framework/l0/levelzero.h"
#include "framework/l0/utility/usm_helper.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/usm_memory_allocation.h"

#include <gtest/gtest.h>

static TestResult run(const UsmMemoryAllocationArguments &arguments, Statistics &statistics) {
    // Setup
    LevelZero levelzero;
    Timer timer;

    // Warmup
    void *ptr{};
    ASSERT_ZE_RESULT_SUCCESS(UsmHelper::allocate(arguments.usmMemoryPlacement, levelzero, arguments.size, &ptr));
    ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, ptr));

    // Benchmark
    for (auto j = 0u; j < arguments.iterations; j++) {
        timer.measureStart();
        ASSERT_ZE_RESULT_SUCCESS(UsmHelper::allocate(arguments.usmMemoryPlacement, levelzero, arguments.size, &ptr));
        timer.measureEnd();
        ASSERT_ZE_RESULT_SUCCESS(zeMemFree(levelzero.context, ptr));

        statistics.pushValue(timer.get(), MeasurementUnit::Microseconds, MeasurementType::Cpu);
    }
    return TestResult::Success;
}

static RegisterTestCaseImplementation<UsmMemoryAllocation> registerTestCase(run, Api::L0);
