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

#include "framework/enum/atomic_memory_order.h"
#include "framework/enum/atomic_scope.h"
#include "framework/enum/data_type.h"
#include "framework/enum/math_operation.h"

#include <cstddef>

class CompilerOptionsBuilder;
struct MathOperationTestData;

struct KernelHelper {
    static MathOperationTestData getDataForKernel(DataType dataType, MathOperation operation, size_t totalThreadsCount);
    static std::string getCompilerOptions(DataType dataType, MathOperation operation, size_t otherArgumentBufferSize);
    static std::string getCompilerOptionsExplicit(DataType dataType, MathOperation operation, AtomicMemoryOrder order,
                                                  AtomicScope scope, size_t otherArgumentBufferSize);

  private:
    static void addAtomicOpMacro(CompilerOptionsBuilder &options, MathOperation operation);
    static void addExplicitAtomicOpMacro(CompilerOptionsBuilder &options, MathOperation operation, AtomicMemoryOrder order, AtomicScope scope);
};
