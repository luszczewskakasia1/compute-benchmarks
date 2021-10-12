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
#include "framework/argument/enum/usm_initial_placement_argument.h"
#include "framework/test_case/test_case.h"

struct UsmSharedFirstCpuAccessArguments : TestCaseArgumentContainer {
    UsmInitialPlacementArgument initialPlacement;
    ByteSizeArgument bufferSize;

    UsmSharedFirstCpuAccessArguments()
        : initialPlacement(*this, "initialPlacement", "Hint for initial placement of the resource passed to the driver"),
          bufferSize(*this, "size", "Size of the buffer") {}
};

struct UsmSharedFirstCpuAccess : TestCase<UsmSharedFirstCpuAccessArguments> {
    using TestCase<UsmSharedFirstCpuAccessArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmSharedFirstCpuAccess";
    }

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to access it on CPU after creation.";
    }
};
