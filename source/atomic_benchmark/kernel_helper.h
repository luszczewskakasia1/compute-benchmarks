#pragma once

#include "framework/enum/atomic_memory_order.h"
#include "framework/enum/atomic_operation.h"
#include "framework/enum/atomic_scope.h"
#include "framework/enum/data_type.h"

#include <cstddef>

struct KernelHelper {
    struct DataForKernel {
        size_t sizeOfDataType;
        std::byte initialValue[8];
        std::byte otherArgument[8];
        std::byte expectedValue[8];
    };

    static DataForKernel getDataForKernel(DataType dataType, AtomicOperation operation, size_t iterations);
    static std::string getCompilerOptions(DataType dataType, AtomicOperation operation, size_t otherArgumentBufferSize);
    static std::string getCompilerOptionsExplicit(DataType dataType, AtomicOperation operation, AtomicMemoryOrder order,
                                                  AtomicScope scope, size_t otherArgumentBufferSize);

  private:
    static std::string getCompilerOption(const std::string &key, const std::string &value);
    static std::string getCompilerOptionForAtomicOp(AtomicOperation operation);
    static std::string getCompilerOptionForAtomicOpExplicit(AtomicOperation operation, AtomicMemoryOrder order, AtomicScope scope);
};
