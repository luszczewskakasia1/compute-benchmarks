#pragma once

#include "framework/argument/abstract/enum_argument.h"
#include "framework/enum/usm_memory_placement.h"

struct UsmMemoryPlacementArgument : EnumArgument<UsmMemoryPlacementArgument, UsmMemoryPlacement> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        markAsParsed();
        return *this;
    }

    const static inline std::string enumName = "memory placement";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[4] = {EnumType::Device, EnumType::Host, EnumType::Shared, EnumType::NonUsm};
    const static inline std::string enumValuesNames[4] = {"Device", "Host", "Shared", "non-USM"};
};
