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
#include "framework/argument/enum/atomic_math_operation_argument.h"
#include "framework/argument/enum/atomic_memory_order_argument.h"
#include "framework/argument/enum/atomic_scope_argument.h"
#include "framework/argument/enum/data_type_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct OneLocalAtomicExplicitArguments : TestCaseArgumentContainer {
    DataTypeArgument dataType;
    AtomicMathOperationArgument atomicOperation;
    AtomicScopeArgument scope;
    AtomicMemoryOrderArgument memoryOrder;
    PositiveIntegerArgument workgroupSize;

    OneLocalAtomicExplicitArguments()
        : dataType(*this, "type", CommonHelpMessage::atomicDataType()),
          atomicOperation(*this, "op", "Atomic operation to perform"),
          scope(*this, "scope", "Memory scope of an atomic operation"),
          memoryOrder(*this, "order", "Memory order of an atomic operation"),
          workgroupSize(*this, "wgs", "Work group size") {}
};

struct OneLocalAtomicExplicit : TestCase<OneLocalAtomicExplicitArguments> {
    using TestCase<OneLocalAtomicExplicitArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "OneLocalAtomicExplicit";
    }

    std::string getHelp() const override {
        return "enqueues kernel performing an atomic operation on a single location placed in SLM "
               "using OpenCL 2.0 Atomics with explicit memory order and scope";
    }
};
