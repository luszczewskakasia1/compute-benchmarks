#pragma once

#include "framework/enum/atomic_operation.h"
#include "framework/enum/data_type.h"

namespace KernelHelper {
struct DataForKernel {
    size_t sizeOfDataType;
    std::byte initialValue[8];
    std::byte otherArgument[8];
    std::byte expectedValue[8];
};

DataForKernel getDataForKernel(DataType dataType, AtomicOperation operation, size_t iterations);
std::string getCompilerOptions(DataType dataType,
                                      AtomicOperation operation,
                                      size_t otherArgumentBufferSize,
                                      const std::string &otherArgumentName = "otherArgument");
} // namespace KernelHelper
