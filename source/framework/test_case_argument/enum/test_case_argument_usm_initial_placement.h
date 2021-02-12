#pragma once

#include "framework/enum/usm_initial_placement.h"
#include "framework/test_case_argument/abstract/test_case_argument_enum.h"

struct UsmInitialPlacementArgument : EnumArgument<UsmInitialPlacementArgument, UsmInitialPlacement> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "USM initial placement";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[3] = {EnumType::Any, EnumType::Host, EnumType::Device};
    const static inline std::string enumValuesNames[3] = {"Any", "Host", "Device"};
};
