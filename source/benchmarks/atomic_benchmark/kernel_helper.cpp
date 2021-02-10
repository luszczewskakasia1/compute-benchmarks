#include "kernel_helper.h"

#include "framework/utility/atomic_operation_helper.h"

#include <algorithm>

template <typename T>
constexpr inline KernelHelper::DataForKernel getDataForKernel(AtomicOperation operation, size_t totalThreadsCount) {
    const auto loopIterations = 100u;                   // Kernel performs some number of loop iterations
    const auto operatorApplicationsPerIteration = 128u; // Each iteration performs some number of atomic operations
    const auto totalAtomicOperationsCount = totalThreadsCount * loopIterations * operatorApplicationsPerIteration;

    KernelHelper::DataForKernel result{};
    result.sizeOfDataType = sizeof(T);
    result.loopIterations = loopIterations;
    T &initialValue = reinterpret_cast<T &>(result.initialValue);
    T &otherArgument = reinterpret_cast<T &>(result.otherArgument);
    T &expectedValue = reinterpret_cast<T &>(result.expectedValue);

    switch (operation) {
    case AtomicOperation::Add:
        initialValue = 1000;
        otherArgument = 3;
        expectedValue = initialValue + otherArgument * static_cast<T>(totalAtomicOperationsCount);
        break;
    case AtomicOperation::Sub:
        initialValue = 1000;
        otherArgument = 3;
        expectedValue = initialValue - otherArgument * static_cast<T>(totalAtomicOperationsCount);
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
        expectedValue = initialValue + static_cast<T>(totalAtomicOperationsCount);
        break;
    case AtomicOperation::Dec:
        initialValue = 43;
        otherArgument = 0;
        expectedValue = initialValue - static_cast<T>(totalAtomicOperationsCount);
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
                expectedValue = (totalAtomicOperationsCount % 2 == 0) ? initialValue : initialValue ^ otherArgument;
                break;
            default:
                FATAL_ERROR("Invalid atomic operation");
            }
        } else {
            FATAL_ERROR("Invalid atomic operation");
        }
    }
    return result;
}

KernelHelper::DataForKernel KernelHelper::getDataForKernel(DataType dataType,
                                                           AtomicOperation operation,
                                                           size_t totalThreadsCount) {

    switch (dataType) {
    case DataType::Float:
        return ::getDataForKernel<float>(operation, totalThreadsCount);
    case DataType::Int32:
        return ::getDataForKernel<int32_t>(operation, totalThreadsCount);
    default:
        FATAL_ERROR("Invalid data type");
    }
}

std::string KernelHelper::getCompilerOptions(DataType dataType, AtomicOperation operation, size_t otherArgumentBufferSize) {
    std::ostringstream result{};
    result << getCompilerOptionForAtomicOp(operation) << " "
           << getCompilerOption("ATOMIC_DATATYPE", DataTypeHelper::toOpenclC(dataType)) << " "
           << getCompilerOption("DATATYPE", DataTypeHelper::toOpenclC(dataType)) << " "
           << getCompilerOption("OTHER_ARGUMENT_BUFFER_SIZE", std::to_string(otherArgumentBufferSize)) << " ";
    return result.str();
}

std::string KernelHelper::getCompilerOptionsExplicit(DataType dataType, AtomicOperation operation, AtomicMemoryOrder order,
                                                     AtomicScope scope, size_t otherArgumentBufferSize) {
    std::ostringstream result{};
    result << getCompilerOptionForAtomicOpExplicit(operation, order, scope) << " "
           << getCompilerOption("ATOMIC_DATATYPE", DataTypeHelper::toExplicitAtomicOpenclC(dataType)) << " "
           << getCompilerOption("DATATYPE", DataTypeHelper::toOpenclC(dataType)) << " "
           << getCompilerOption("OTHER_ARGUMENT_BUFFER_SIZE", std::to_string(otherArgumentBufferSize)) << " "
           << "-DOCL_20 "
           << "-cl-std=CL2.0";
    return result.str();
}

std::string KernelHelper::getCompilerOption(const std::string &key, const std::string &value) {
    std::ostringstream result{};
    result << "-D " << key << "=" << value;
    return result.str();
}

std::string KernelHelper::getCompilerOptionForAtomicOp(AtomicOperation operation) {
    const static char *functionNames[] = {"ERROR",
                                          "atomic_add", "atomic_sub",
                                          "atomic_xchg", "atomic_cmpxchg",
                                          "atomic_inc", "atomic_dec",
                                          "atomic_min", "atomic_max",
                                          "atomic_and", "atomic_or",
                                          "atomic_xor"};

    std::ostringstream result{};
    result << "-D ATOMIC_OP(address,other)="
           << functionNames[static_cast<int>(operation)]
           << "(address";
    switch (operation) {
    case AtomicOperation::Add:
    case AtomicOperation::Sub:
    case AtomicOperation::Min:
    case AtomicOperation::Max:
    case AtomicOperation::And:
    case AtomicOperation::Or:
    case AtomicOperation::Xor:
    case AtomicOperation::Xchg:
        result << ",other";
        break;
    case AtomicOperation::Inc:
    case AtomicOperation::Dec:
        break;
    case AtomicOperation::CmpXchg:
        result << ",other,other";
        break;
    default:
        FATAL_ERROR("Unknown atomic operation");
    }
    result << ")";
    return result.str();
}

std::string KernelHelper::getCompilerOptionForAtomicOpExplicit(AtomicOperation operation, AtomicMemoryOrder order, AtomicScope scope) {
    const static char *functionNames[] = {"ERROR",
                                          "atomic_fetch_add_explicit", "atomic_fetch_sub_explicit",
                                          "atomic_exchange_explicit", "atomic_compare_exchange_strong_explicit",
                                          "atomic_fetch_add_explicit", "atomic_fetch_sub_explicit", // inc and dec emulated with add and sub
                                          "atomic_fetch_min_explicit", "atomic_fetch_max_explicit",
                                          "atomic_fetch_and_explicit", "atomic_fetch_or_explicit",
                                          "atomic_fetch_xor_explicit"};

    std::ostringstream result{};
    result << "-D ATOMIC_OP(address,other)="
           << functionNames[static_cast<int>(operation)]
           << "(address";
    switch (operation) {
    case AtomicOperation::Add:
    case AtomicOperation::Sub:
    case AtomicOperation::Min:
    case AtomicOperation::Max:
    case AtomicOperation::And:
    case AtomicOperation::Or:
    case AtomicOperation::Xor:
    case AtomicOperation::Xchg:
        result << ",other";
        break;
    case AtomicOperation::Inc:
    case AtomicOperation::Dec:
        result << ",1";
        break;

    case AtomicOperation::CmpXchg:
        result << ",&other,other," << AtomicMemoryOrderHelper::toOpenclC(order);
        break;
    default:
        FATAL_ERROR("Unknown atomic operation");
    }

    result << "," << AtomicMemoryOrderHelper::toOpenclC(order)
           << "," << AtomicScopeHelper::toOpenclC(scope)
           << ")";
    return result.str();
}
