#pragma once

#include "framework/enum/data_type.h"
#include "framework/enum/math_operation.h"

struct MathOperationHelper {
    static bool isSupportedAsAtomic(MathOperation operation, DataType type, bool globalAtomicFloatsSupported);
    static bool isSupportedAsNormal(MathOperation operation, DataType type);
    static size_t getArgumentsCount(MathOperation operation);
};
