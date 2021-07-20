#include "framework/utility/math_operation_helper.h"

#include "framework/utility/error.h"

size_t MathOperationHelper::getArgumentsCount(MathOperation operation) {
    switch (operation) {
    case MathOperation::Unknown:
        FATAL_ERROR("Unknown math operation");
    case MathOperation::Inc:
    case MathOperation::Dec:
        return 1;
    default:
        return 2;
    case MathOperation::CmpXchg:
        return 3;
    }

    return operation == MathOperation::Inc || operation == MathOperation::Dec;
}

bool MathOperationHelper::isSupportedAsAtomic(MathOperation operation, DataType type, bool globalAtomicFloatsSupported) {
    if (operation == MathOperation::Div || operation == MathOperation::Modulo) {
        return false;
    }

    switch (type) {
    case DataType::Float:
        switch (operation) {
        case MathOperation::Unknown:
            FATAL_ERROR("Unknown math operation");
        case MathOperation::Xchg:
            return true;
        case MathOperation::Add:
        case MathOperation::Sub:
            return globalAtomicFloatsSupported;
        default:
            return false;
        }
    case DataType::Int32:
        return true;
    default:
        FATAL_ERROR("Unknown data type");
    }
}

bool MathOperationHelper::isSupportedAsNormal(MathOperation operation, DataType type) {
    switch (operation) {
    case MathOperation::Unknown:
        FATAL_ERROR("Unknown math operation");
    case MathOperation::Xchg:
    case MathOperation::CmpXchg:
        return false;
    case MathOperation::And:
    case MathOperation::Or:
    case MathOperation::Xor:
    case MathOperation::Modulo:
        return type != DataType::Float;
    default:
        return true;
    }
}

MathOperationTestData MathOperationHelper::generateTestData(DataType dataType, MathOperation operation, size_t loopIterations,
                                                            size_t operationsPerLoop, size_t totalThreadsCount) {
    switch (dataType) {
    case DataType::Float:
        return MathOperationHelper::generateTestData<float>(operation, loopIterations, operationsPerLoop, totalThreadsCount);
    case DataType::Int32:
        return MathOperationHelper::generateTestData<int32_t>(operation, loopIterations, operationsPerLoop, totalThreadsCount);
    default:
        FATAL_ERROR("Invalid data type");
    }
}
