#pragma once

#include "framework/enum/atomic_operation.h"

struct AtomicOperationHelper {
    struct DataForKernel {
        size_t sizeOfDataType;
        std::byte initialValue[8];
        std::byte otherArgument[8];
        std::byte expectedValue[8];
    };

    static DataForKernel getDataForKernel(DataType dataType, AtomicOperation operation, size_t iterations);
    static std::string getCompilerOptions(AtomicOperation operation, const std::string &otherArgumentName = "otherArgument");

  private:
    static size_t getArgumentsCount(AtomicOperation operation);
};
