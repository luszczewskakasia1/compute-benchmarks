#pragma once

#include "framework/enum/map_flags.h"
#include "framework/test_case_argument/abstract/test_case_argument_enum.h"

struct MapFlagsTestCaseArgument : EnumTestCaseArgument<MapFlagsTestCaseArgument, MapFlags> {
    using EnumTestCaseArgument::EnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "map flag";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[3] = {EnumType::Read, EnumType::Write, EnumType::WriteInvalidate};
    const static inline std::string enumValuesNames[3] = {"Read", "Write", "WriteInvalidate"};
};
