#pragma once

#include "framework/enum/api.h"
#include "framework/test_case_argument/abstract/test_case_argument_enum.h"

struct ApiArgument : EnumArgument<ApiArgument, Api> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "api";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[3] = {EnumType::OpenCL, EnumType::L0, EnumType::All};
    const static inline std::string enumValuesNames[3] = {"ocl", "l0", "all"};
};
