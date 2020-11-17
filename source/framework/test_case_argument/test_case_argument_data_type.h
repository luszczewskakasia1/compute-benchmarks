#pragma once

#include "framework/enum/data_type.h"
#include "framework/test_case_argument/abstract/test_case_argument_enum.h"

struct DataTypeTestCaseArgument : EnumTestCaseArgument<DataTypeTestCaseArgument, DataType> {
    using EnumTestCaseArgument::EnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "data type";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[2] = {EnumType::Int32, EnumType::Float};
    const static inline std::string enumValuesNames[2] = {"Int32", "Float"};
};
