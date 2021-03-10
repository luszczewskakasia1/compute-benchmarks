#pragma once

#include "framework/enum/atomic_memory_order.h"
#include "framework/enum/atomic_operation.h"
#include "framework/enum/atomic_scope.h"
#include "framework/enum/data_type.h"

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

    static DataForKernel getDataForKernel(DataType dataType, AtomicOperation operation, size_t totalThreadsCount);
    static std::string getCompilerOptions(DataType dataType, AtomicOperation operation, size_t otherArgumentBufferSize);
    static std::string getCompilerOptionsExplicit(DataType dataType, AtomicOperation operation, AtomicMemoryOrder order,
                                                  AtomicScope scope, size_t otherArgumentBufferSize);

  private:
    static void addAtomicOpMacro(CompilerOptionsBuilder &options, AtomicOperation operation);
    static void addExplicitAtomicOpMacro(CompilerOptionsBuilder &options, AtomicOperation operation, AtomicMemoryOrder order, AtomicScope scope);
};
