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
#include "framework/argument/enum/data_type_argument.h"
#include "framework/argument/enum/normal_math_operation_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct ReadAfterAtomicWriteArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument workgroupSize;
    BooleanArgument atomic;
    BooleanArgument shuffleRead;

    ReadAfterAtomicWriteArguments()
        : workgroupSize(*this, "wgs", "Workgroup size"),
          atomic(*this, "atomic", "If true, write to global memory will be atomic."),
          shuffleRead(*this, "shuffleRead", "If true, each thread will write and read different memory cell. Otherwise it will be the same one.") {}
};

struct ReadAfterAtomicWrite : TestCase<ReadAfterAtomicWriteArguments> {
    using TestCase<ReadAfterAtomicWriteArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "ReadAfterAtomicWrite";
    }

    std::string getHelp() const override {
        return "enqueues kernel, which writes to global memory using atomic and then reads non atomically";
    }
};
