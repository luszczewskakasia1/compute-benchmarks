#pragma once

#include "framework/enum/atomic_operation.h"
#include "framework/enum/data_type.h"

namespace AtomicOperationHelper {
bool isSupported(AtomicOperation operation, DataType type);
size_t getArgumentsCount(AtomicOperation operation);
}; // namespace AtomicOperationHelper
