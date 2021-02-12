#pragma once

#include "framework/argument/abstract/enum_argument.h"
#include "framework/enum/memory_placement.h"

struct MemoryPlacementArgument : EnumArgument<MemoryPlacementArgument, MemoryPlacement> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "memory placement";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[3] = {EnumType::Device, EnumType::Host, EnumType::Shared};
    const static inline std::string enumValuesNames[3] = {"Device", "Host", "Shared"};
};
