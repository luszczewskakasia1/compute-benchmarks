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
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct UsmSharedMigrateCpuArguments : TestCaseArgumentContainer {
    BooleanArgument accessAllBytes;
    ByteSizeArgument bufferSize;

    UsmSharedMigrateCpuArguments()
        : accessAllBytes(*this, "accessAllBytes", "Select, whether entire resource or only one byte will be accessed on CPU"),
          bufferSize(*this, "size", "Size of the buffer") {}
};

struct UsmSharedMigrateCpu : TestCase<UsmSharedMigrateCpuArguments> {
    using TestCase<UsmSharedMigrateCpuArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "UsmSharedMigrateCpu";
    }

    std::string getHelp() const override {
        return "allocates a unified shared memory buffer and measures time to migrate it from GPU to CPU";
    }
};
