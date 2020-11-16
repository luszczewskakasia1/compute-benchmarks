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

namespace AtomicOperationHelper {
const inline char *functionNames[] = {"ERROR",
                                      "atomic_add", "atomic_sub",
                                      "atomic_xchg", "atomic_cmpxchg",
                                      "atomic_inc", "atomic_dec",
                                      "atomic_min", "atomic_max",
                                      "atomic_and", "atomic_or",
                                      "atomic_xor"};

const inline size_t getArgumentsCount(AtomicOperation operation) {
    switch (operation) {
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
    result << "-D DATATYPE=uint";

    return result.str();
}

} // namespace AtomicOperationHelper
