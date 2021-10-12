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

struct DoMathOperationArguments : TestCaseArgumentContainer {
    DataTypeArgument dataType;
    NormalMathOperationArgument operation;
    PositiveIntegerArgument workgroupCount;
    PositiveIntegerArgument workgroupSize;

    DoMathOperationArguments()
        : dataType(*this, "type", CommonHelpMessage::atomicDataType()),
          operation(*this, "op", "Math operation to perform"),
          workgroupCount(*this, "wgc", "Work group count"),
          workgroupSize(*this, "wgs", "Work group size") {}
};

struct DoMathOperation : TestCase<DoMathOperationArguments> {
    using TestCase<DoMathOperationArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "DoMathOperation";
    }

    std::string getHelp() const override {
        return "enqueues kernel performing a math operation";
    }
};
