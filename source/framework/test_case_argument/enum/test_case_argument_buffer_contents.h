#pragma once

#include "framework/enum/buffer_contents.h"
#include "framework/test_case_argument/abstract/test_case_argument_enum.h"

struct BufferContentsTestCaseArgument : EnumTestCaseArgument<BufferContentsTestCaseArgument, BufferContents> {
    using EnumTestCaseArgument::EnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "buffer contents";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[2] = {EnumType::Zeros, EnumType::Random};
    const static inline std::string enumValuesNames[2] = {"Zeros", "Random"};
};
