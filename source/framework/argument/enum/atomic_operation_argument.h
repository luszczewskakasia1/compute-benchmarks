#pragma once

#include "framework/argument/abstract/enum_argument.h"
#include "framework/enum/atomic_operation.h"

struct AtomicOperationArgument : EnumArgument<AtomicOperationArgument, AtomicOperation> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        markAsParsed();
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
