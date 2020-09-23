#pragma once

#include "framework/enum/memory_placement.h"
#include "framework/test_case_argument/test_case_argument_enum.h"

struct MemoryPlacementTestCaseArgument : EnumTestCaseArgument<MemoryPlacementTestCaseArgument, MemoryPlacement> {
    using EnumTestCaseArgument::EnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "memory placement";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[2] = {EnumType::Device, EnumType::Host};
    const static inline std::string enumValuesNames[2] = {"Device", "Host"};
};
