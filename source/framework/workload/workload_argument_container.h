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

#include "framework/argument/argument_container.h"
#include "framework/argument/basic_argument.h"

struct WorkloadArgumentContainer : ArgumentContainer {
    PositiveIntegerArgument iterations;
    BooleanArgument synchronize;
    IntegerArgument synchronizationPipeIn;
    IntegerArgument synchronizationPipeOut;
    IntegerArgument measurementPipe;

    WorkloadArgumentContainer()
        : iterations(*this, "iterations", "Number of iterations to perform"),
          synchronize(*this, "synchronize", "Wait for synchronization before each iteration"),
          synchronizationPipeIn(*this, "synchronizationPipeIn", "Handle for the synchronization pipe (parent to child). If 0, stdin is used."),
          synchronizationPipeOut(*this, "synchronizationPipeOut", "Handle for the synchronization pipe (child to parent). If 0, stdout is used."),
          measurementPipe(*this, "measurementPipe", "Handle for the measurements pipe. If 0, stdout is used") {

        // Default values
        iterations = 10;

        // Default values for parameters used by benchmarks calling the workload. Not meant to be used by humans.
        synchronize = false;
        synchronizationPipeIn = 0;
        synchronizationPipeOut = 0;
        measurementPipe = 0;
    }
};
