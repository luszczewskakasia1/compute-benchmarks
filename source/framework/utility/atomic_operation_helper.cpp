#include "framework/utility/atomic_operation_helper.h"

#include "framework/utility/error.h"

#include <algorithm>

template <typename T>
constexpr inline AtomicOperationHelper::DataForKernel getDataForKernel(AtomicOperation operation, size_t iterations) {
    AtomicOperationHelper::DataForKernel result{};
    result.sizeOfDataType = sizeof(T);
    T &initialValue = reinterpret_cast<T &>(result.initialValue);
    T &otherArgument = reinterpret_cast<T &>(result.otherArgument);
    T &expectedValue = reinterpret_cast<T &>(result.expectedValue);

    switch (operation) {
    case AtomicOperation::Add:
        initialValue = 1000;
        otherArgument = 3;
        expectedValue = initialValue + otherArgument * static_cast<T>(iterations);
        break;
    case AtomicOperation::Sub:
        initialValue = 1000;
        otherArgument = 3;
        expectedValue = initialValue - otherArgument * static_cast<T>(iterations);
        break;
    case AtomicOperation::Xchg:
        initialValue = 7;
        otherArgument = 3;
        expectedValue = otherArgument;
        break;
    case AtomicOperation::CmpXchg:
        initialValue = 7;
        otherArgument = 3;
        expectedValue = initialValue;
        break;
    case AtomicOperation::Inc:
        initialValue = 12;
        otherArgument = 0;
        expectedValue = initialValue + static_cast<T>(iterations);
        break;
    case AtomicOperation::Dec:
        initialValue = 43;
        otherArgument = 0;
        expectedValue = initialValue - static_cast<T>(iterations);
        break;
    case AtomicOperation::Min:
        initialValue = 100;
        otherArgument = 33;
        expectedValue = std::min(initialValue, otherArgument);
        break;
    case AtomicOperation::Max:
        initialValue = 100;
        otherArgument = 33;
        expectedValue = std::max(initialValue, otherArgument);
        break;
    default:
        if constexpr (std::is_integral_v<T>) {
            switch (operation) {
            case AtomicOperation::And:
                initialValue = 0b1111001111001101;
                otherArgument = 0b1101100100101010;
                expectedValue = initialValue & otherArgument;
                break;
            case AtomicOperation::Or:
                initialValue = 0b1111001111001101;
                otherArgument = 0b1101100100101010;
                expectedValue = initialValue | otherArgument;
                break;
            case AtomicOperation::Xor:
                initialValue = 0b1111001111001101;
                otherArgument = 0b1101100100101010;
                expectedValue = (iterations % 2 == 0) ? initialValue : initialValue ^ otherArgument;
                break;
            default:
                ERROR("Invalid atomic operation");
            }
        } else {
            ERROR("Invalid atomic operation");
        }
    }
    return result;
}

AtomicOperationHelper::DataForKernel AtomicOperationHelper::getDataForKernel(DataType dataType,
                                                                             AtomicOperation operation,
                                                                             size_t iterations) {
    switch (dataType) {
    case DataType::Float:
        return ::getDataForKernel<float>(operation, iterations);
    case DataType::Int32:
        return ::getDataForKernel<int32_t>(operation, iterations);
    default:
        ERROR("Invalid data type");
    }
}

size_t AtomicOperationHelper::getArgumentsCount(AtomicOperation operation) {
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

std::string AtomicOperationHelper::getCompilerOptions(AtomicOperation operation, const std::string &otherArgumentName) {
    const static char *functionNames[] = {"ERROR",
                                          "atomic_add", "atomic_sub",
                                          "atomic_xchg", "atomic_cmpxchg",
                                          "atomic_inc", "atomic_dec",
                                          "atomic_min", "atomic_max",
                                          "atomic_and", "atomic_or",
                                          "atomic_xor"};

    std::ostringstream result{};

    // Select atomic operation
    result << "-D ATOMIC_OP(address," << otherArgumentName << ")="
           << functionNames[static_cast<int>(operation)] << "(address";
    if (getArgumentsCount(operation) >= 2) {
        result << "," << otherArgumentName;
    }
    if (getArgumentsCount(operation) >= 3) {
        result << "," << otherArgumentName;
    }
    result << ") ";

    // Select datatype (TODO: it's hardcoded for now)
    result << "-D DATATYPE=int";

    return result.str();
}
