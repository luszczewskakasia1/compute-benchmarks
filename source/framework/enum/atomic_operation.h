#pragma once

#include <sstream>
#include <string>

enum class AtomicOperation {
    Unknown,
    Add,
    Sub,
    Xchg,
    CmpXchg,
    Inc,
    Dec,
    Min,
    Max,
    And,
    Or,
    Xor,
};

enum class DataType {
    Unknown,
    Int32,
    Float,
};

namespace AtomicOperationHelper {
const inline char *functionNames[] = {"ERROR",
                                      "atomic_add", "atomic_sub",
                                      "atomic_xchg", "atomic_cmpxchg",
                                      "atomic_inc", "atomic_dec",
                                      "atomic_min", "atomic_max",
                                      "atomic_and", "atomic_or",
                                      "atomic_xor"};

template <typename T>
constexpr inline T apply(T initialValue, AtomicOperation operation, T otherArgument, size_t iterations) {
    switch (operation) {
    case AtomicOperation::Add:
        return initialValue + otherArgument * static_cast<T>(iterations);
    case AtomicOperation::Sub:
        return initialValue - otherArgument * static_cast<T>(iterations);
    case AtomicOperation::Xchg:
        return otherArgument;
    case AtomicOperation::CmpXchg:
        return initialValue;
    case AtomicOperation::Inc:
        return initialValue + static_cast<T>(iterations);
    case AtomicOperation::Dec:
        return initialValue - static_cast<T>(iterations);
    case AtomicOperation::Min:
        return std::min(initialValue, otherArgument);
    case AtomicOperation::Max:
        return std::max(initialValue, otherArgument);
    }

    if constexpr (std::is_integral_v<T>) {
        switch (operation) {
        case AtomicOperation::And:
            return initialValue & otherArgument;
        case AtomicOperation::Or:
            return initialValue | otherArgument;
        case AtomicOperation::Xor:
            return (iterations % 2 == 0) ? initialValue : initialValue ^ otherArgument;
        }
    }

    ERROR("Invalid atomic operation");
}

struct DataForKernel {
    size_t sizeOfDataType;
    std::byte initialValue[8];
    std::byte otherArgument[8];
    std::byte expectedValue[8];
};

template <typename T>
constexpr DataForKernel getDataForKernel(AtomicOperation operation, size_t iterations) {
    DataForKernel result{};
    result.sizeOfDataType = sizeof(T);
    T &initialValue = reinterpret_cast<T &>(result.initialValue);
    T &otherArgument = reinterpret_cast<T &>(result.otherArgument);
    T &expectedValue = reinterpret_cast<T &>(result.expectedValue);

    switch (operation) {
    case AtomicOperation::Add:
    case AtomicOperation::Sub:
        initialValue = 1000;
        otherArgument = 3;
        break;
    case AtomicOperation::Xchg:
    case AtomicOperation::CmpXchg:
        initialValue = 7;
        otherArgument = 3;
        break;
    case AtomicOperation::Inc:
    case AtomicOperation::Dec:
        initialValue = 12;
        otherArgument = 0;
        break;
    case AtomicOperation::Min:
    case AtomicOperation::Max:
        initialValue = 100;
        otherArgument = 33;
        break;
    case AtomicOperation::And:
    case AtomicOperation::Or:
    case AtomicOperation::Xor:
        initialValue = 0b1111001111001101;
        otherArgument = 0b1101100100101010;
        break;
    default:
        ERROR("Invalid atomic operation");
    }

    expectedValue = AtomicOperationHelper::apply(initialValue, operation, otherArgument, iterations);

    return result;
}

constexpr DataForKernel getDataForKernel(DataType dataType, AtomicOperation operation, size_t iterations) {
    switch (dataType) {
    case DataType::Float:
        return getDataForKernel<float>(operation, iterations);
    case DataType::Int32:
        return getDataForKernel<int32_t>(operation, iterations);
    default:
        ERROR("Invalid data type");
    }
}

const inline size_t getArgumentsCount(AtomicOperation operation) {
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

const inline std::string getCompilerOptions(AtomicOperation operation, const std::string &otherArgumentName = "otherArgument") {
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

} // namespace AtomicOperationHelper
