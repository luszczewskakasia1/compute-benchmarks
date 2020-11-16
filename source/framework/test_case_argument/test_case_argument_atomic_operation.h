#pragma once

#include "framework/enum/atomic_operation.h"
#include "framework/test_case_argument/abstract/test_case_argument_enum.h"

struct AtomicOperationTestCaseArgument : EnumTestCaseArgument<AtomicOperationTestCaseArgument, AtomicOperation> {
    using EnumTestCaseArgument::EnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "atomic operation";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[11] = {EnumType::Add, EnumType::Sub,
                                                   EnumType::Xchg, EnumType::CmpXchg,
                                                   EnumType::Inc, EnumType::Dec,
                                                   EnumType::Min, EnumType::Max,
                                                   EnumType::And, EnumType::Or,
                                                   EnumType::Xor};
    const static inline std::string enumValuesNames[11] = {"Add", "Sub",
                                                           "Xchg", "CmpXchg",
                                                           "Inc", "Dec",
                                                           "Min", "Max",
                                                           "And", "Or",
                                                           "Xor"};
};
