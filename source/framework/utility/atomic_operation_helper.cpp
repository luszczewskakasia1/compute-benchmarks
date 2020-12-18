#include "framework/utility/atomic_operation_helper.h"

#include "framework/utility/error.h"

namespace AtomicOperationHelper {

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

bool isSupported(AtomicOperation operation, DataType type, bool globalAtomicFloatsSupported) {
    switch (type) {
    case DataType::Float:
        switch (operation) {
        case AtomicOperation::Xchg:
            return true;
        case AtomicOperation::Add:
        case AtomicOperation::Sub:
            return globalAtomicFloatsSupported;
        default:
            return false;
        }
    case DataType::Int32:
        return true;
    default:
        ERROR("Unknown data type");
    }
}
} // namespace AtomicOperationHelper
