#include "framework/utility/atomic_operation_helper.h"

#include "framework/utility/error.h"

namespace AtomicOperationHelper {
std::string getDataTypeName(DataType dataType) {
    switch (dataType) {
    case DataType::Float:
        return "float";
    case DataType::Int32:
        return "int";
    default:
        ERROR("Unknown data type");
    }
}

size_t getArgumentsCount(AtomicOperation operation) {
    switch (operation) {
    case AtomicOperation::Unknown:
        ERROR("Unknown atomic operation");
    case AtomicOperation::Inc:
    case AtomicOperation::Dec:
        return 1;
    default:
        return 2;
    case AtomicOperation::CmpXchg:
        return 3;
    }

    return operation == AtomicOperation::Inc || operation == AtomicOperation::Dec;
}

bool isSupported(AtomicOperation operation, DataType type) {
    switch (type) {
    case DataType::Float:
        return operation == AtomicOperation::Xchg;
    case DataType::Int32:
        return true;
    default:
        ERROR("Unknown data type");
    }
}
} // namespace AtomicOperationHelper
