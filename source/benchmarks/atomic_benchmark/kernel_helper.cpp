#include "kernel_helper.h"

#include "framework/utility/compiler_options_builder.h"
#include "framework/utility/math_operation_helper.h"

#include <algorithm>

template <typename T>
constexpr inline KernelHelper::DataForKernel getDataForKernel(MathOperation operation, size_t totalThreadsCount) {
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
    case MathOperation::Add:
        initialValue = 1000;
        otherArgument = 3;
        expectedValue = initialValue + otherArgument * static_cast<T>(totalAtomicOperationsCount);
        break;
    case MathOperation::Sub:
        initialValue = 1000;
        otherArgument = 3;
        expectedValue = initialValue - otherArgument * static_cast<T>(totalAtomicOperationsCount);
        break;
    case MathOperation::Xchg:
        initialValue = 7;
        otherArgument = 3;
        expectedValue = otherArgument;
        break;
    case MathOperation::CmpXchg:
        initialValue = 7;
        otherArgument = 3;
        expectedValue = initialValue;
        break;
    case MathOperation::Inc:
        initialValue = 12;
        otherArgument = 0;
        expectedValue = initialValue + static_cast<T>(totalAtomicOperationsCount);
        break;
    case MathOperation::Dec:
        initialValue = 43;
        otherArgument = 0;
        expectedValue = initialValue - static_cast<T>(totalAtomicOperationsCount);
        break;
    case MathOperation::Min:
        initialValue = 100;
        otherArgument = 33;
        expectedValue = std::min(initialValue, otherArgument);
        break;
    case MathOperation::Max:
        initialValue = 100;
        otherArgument = 33;
        expectedValue = std::max(initialValue, otherArgument);
        break;
    default:
        if constexpr (std::is_integral_v<T>) {
            switch (operation) {
            case MathOperation::And:
                initialValue = 0b1111001111001101;
                otherArgument = 0b1101100100101010;
                expectedValue = initialValue & otherArgument;
                break;
            case MathOperation::Or:
                initialValue = 0b1111001111001101;
                otherArgument = 0b1101100100101010;
                expectedValue = initialValue | otherArgument;
                break;
            case MathOperation::Xor:
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
                                                           MathOperation operation,
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

std::string KernelHelper::getCompilerOptions(DataType dataType, MathOperation operation, size_t otherArgumentBufferSize) {
    CompilerOptionsBuilder options{};
    addAtomicOpMacro(options, operation);
    options.addDefinitionKeyValue("ATOMIC_DATATYPE", DataTypeHelper::toOpenclC(dataType));
    options.addDefinitionKeyValue("DATATYPE", DataTypeHelper::toOpenclC(dataType));
    options.addDefinitionKeyValue("OTHER_ARGUMENT_BUFFER_SIZE", std::to_string(otherArgumentBufferSize));
    return options.str();
}

std::string KernelHelper::getCompilerOptionsExplicit(DataType dataType, MathOperation operation, AtomicMemoryOrder order,
                                                     AtomicScope scope, size_t otherArgumentBufferSize) {
    CompilerOptionsBuilder options{};
    addExplicitAtomicOpMacro(options, operation, order, scope);
    options.addOptionOpenCl20();
    options.addDefinition("OCL_20");
    options.addDefinitionKeyValue("ATOMIC_DATATYPE", DataTypeHelper::toExplicitAtomicOpenclC(dataType));
    options.addDefinitionKeyValue("DATATYPE", DataTypeHelper::toOpenclC(dataType));
    options.addDefinitionKeyValue("OTHER_ARGUMENT_BUFFER_SIZE", std::to_string(otherArgumentBufferSize));
    return options.str();
}

void KernelHelper::addAtomicOpMacro(CompilerOptionsBuilder &options, MathOperation operation) {
    const static char *functionNames[] = {"ERROR",
                                          "atomic_add", "atomic_sub",
                                          "atomic_xchg", "atomic_cmpxchg",
                                          "atomic_inc", "atomic_dec",
                                          "atomic_min", "atomic_max",
                                          "atomic_and", "atomic_or",
                                          "atomic_xor"};

    std::ostringstream macroBody{};
    macroBody << functionNames[static_cast<int>(operation)] << "(address";
    switch (operation) {
    case MathOperation::Add:
    case MathOperation::Sub:
    case MathOperation::Min:
    case MathOperation::Max:
    case MathOperation::And:
    case MathOperation::Or:
    case MathOperation::Xor:
    case MathOperation::Xchg:
        macroBody << ",other";
        break;
    case MathOperation::Inc:
    case MathOperation::Dec:
        break;
    case MathOperation::CmpXchg:
        macroBody << ",other,other";
        break;
    default:
        FATAL_ERROR("Unknown atomic operation");
    }
    macroBody << ")";

    options.addMacro("ATOMIC_OP", {"address", "other"}, macroBody.str().c_str());
}

void KernelHelper::addExplicitAtomicOpMacro(CompilerOptionsBuilder &options, MathOperation operation, AtomicMemoryOrder order, AtomicScope scope) {
    const static char *functionNames[] = {"ERROR",
                                          "atomic_fetch_add_explicit", "atomic_fetch_sub_explicit",
                                          "atomic_exchange_explicit", "atomic_compare_exchange_strong_explicit",
                                          "atomic_fetch_add_explicit", "atomic_fetch_sub_explicit", // inc and dec emulated with add and sub
                                          "atomic_fetch_min_explicit", "atomic_fetch_max_explicit",
                                          "atomic_fetch_and_explicit", "atomic_fetch_or_explicit",
                                          "atomic_fetch_xor_explicit"};

    std::ostringstream macroBody{};
    macroBody << functionNames[static_cast<int>(operation)] << "(address";
    switch (operation) {
    case MathOperation::Add:
    case MathOperation::Sub:
    case MathOperation::Min:
    case MathOperation::Max:
    case MathOperation::And:
    case MathOperation::Or:
    case MathOperation::Xor:
    case MathOperation::Xchg:
        macroBody << ",other";
        break;
    case MathOperation::Inc:
    case MathOperation::Dec:
        macroBody << ",1";
        break;
    case MathOperation::CmpXchg:
        macroBody << ",&other,other," << AtomicMemoryOrderHelper::toOpenclC(order);
        break;
    default:
        FATAL_ERROR("Unknown atomic operation");
    }
    macroBody << "," << AtomicMemoryOrderHelper::toOpenclC(order)
              << "," << AtomicScopeHelper::toOpenclC(scope)
              << ")";

    options.addMacro("ATOMIC_OP", {"address", "other"}, macroBody.str().c_str());
}
