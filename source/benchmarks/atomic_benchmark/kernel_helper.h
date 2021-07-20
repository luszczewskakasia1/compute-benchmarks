#pragma once

#include "framework/enum/atomic_memory_order.h"
#include "framework/enum/atomic_scope.h"
#include "framework/enum/data_type.h"
#include "framework/enum/math_operation.h"

#include <cstddef>

class CompilerOptionsBuilder;

struct KernelHelper {
    struct DataForKernel {
        size_t sizeOfDataType;
        size_t loopIterations;
        std::byte initialValue[8];
        std::byte otherArgument[8];
        std::byte expectedValue[8];
    };

    static DataForKernel getDataForKernel(DataType dataType, MathOperation operation, size_t totalThreadsCount);
    static std::string getCompilerOptions(DataType dataType, MathOperation operation, size_t otherArgumentBufferSize);
    static std::string getCompilerOptionsExplicit(DataType dataType, MathOperation operation, AtomicMemoryOrder order,
                                                  AtomicScope scope, size_t otherArgumentBufferSize);

  private:
    static void addAtomicOpMacro(CompilerOptionsBuilder &options, MathOperation operation);
    static void addExplicitAtomicOpMacro(CompilerOptionsBuilder &options, MathOperation operation, AtomicMemoryOrder order, AtomicScope scope);
};
