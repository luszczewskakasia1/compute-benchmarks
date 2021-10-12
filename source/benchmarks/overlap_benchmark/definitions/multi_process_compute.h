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

#pragma once

#include "framework/argument/basic_argument.h"
#include "framework/argument/enum/multi_device_selection_argument.h"
#include "framework/test_case/test_case.h"

struct MultiProcessComputeArguments : TestCaseArgumentContainer {
    MultipleTilesSelectionArgument deviceSelection;
    PositiveIntegerArgument processesPerTile;
    PositiveIntegerArgument workgroupsPerProcess;
    PositiveIntegerArgument operationsPerKernelCount;
    BooleanArgument synchronize;

    MultiProcessComputeArguments()
        : deviceSelection(*this, "tiles", "Tiles for execution"),
          processesPerTile(*this, "processesPerTile", "Number of processes that will be started on each of the tiles specified"),
          workgroupsPerProcess(*this, "workgroupsPerProcess", "Number of workgroups that each process will start"),
          synchronize(*this, "synchronize", "Synchronize all processes before each iteration"),
          operationsPerKernelCount(*this, "opsPerKernel", "Operations performed in kernel, used to steer its execution time") {}
};

struct MultiProcessCompute : TestCase<MultiProcessComputeArguments> {
    using TestCase<MultiProcessComputeArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "MultiProcessCompute";
    }

    std::string getHelp() const override {
        return "Creates a number of separate processes for each tile specified performing a "
               "compute workload and measures average time to complete all of them. Processes "
               "will use affinity mask to select specific sub-devices for the execution";
    }
};
