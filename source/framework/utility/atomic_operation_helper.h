#pragma once

#include "framework/enum/atomic_operation.h"
#include "framework/enum/data_type.h"

struct AtomicOperationHelper {
    struct DataForKernel {
        size_t sizeOfDataType;
        std::byte initialValue[8];
        std::byte otherArgument[8];
        std::byte expectedValue[8];
    };

    static DataForKernel getDataForKernel(DataType dataType, AtomicOperation operation, size_t iterations);
    static std::string getCompilerOptions(DataType dataType,
                                          AtomicOperation operation,
                                          size_t otherArgumentBufferSize,
                                          const std::string &otherArgumentName = "otherArgument");
    static bool isSupported(AtomicOperation operation, DataType type);

  private:
    static std::string getDataTypeName(DataType dataType);
    static size_t getArgumentsCount(AtomicOperation operation);
};
