#include "atomic_benchmark/kernel_helper.h"

#include "framework/utility/atomic_operation_helper.h"

#include <algorithm>

namespace KernelHelper {

template <typename T>
constexpr inline DataForKernel getDataForKernel(AtomicOperation operation, size_t iterations) {
    DataForKernel result{};
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

DataForKernel getDataForKernel(DataType dataType,
                               AtomicOperation operation,
                               size_t iterations) {
    switch (dataType) {
    case DataType::Float:
        return getDataForKernel<float>(operation, iterations);
    case DataType::Int32:
        return getDataForKernel<int32_t>(operation, iterations);
    default:
        ERROR("Invalid data type");
    }
}

std::string getCompilerOptions(DataType dataType,
                               AtomicOperation operation,
                               size_t otherArgumentBufferSize,
                               const std::string &otherArgumentName) {
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
    if (AtomicOperationHelper::getArgumentsCount(operation) >= 2) {
        result << "," << otherArgumentName;
    }
    if (AtomicOperationHelper::getArgumentsCount(operation) >= 3) {
        result << "," << otherArgumentName;
    }
    result << ") ";

    // Select datatype
    result << "-D DATATYPE=" << AtomicOperationHelper::getDataTypeName(dataType) << " ";

    // Select otherArguments buffer size
    result << "-D OTHER_ARGUMENT_BUFFER_SIZE=" << otherArgumentBufferSize << " ";

    return result.str();
}
} // namespace KernelHelper
