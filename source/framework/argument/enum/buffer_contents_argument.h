#pragma once

#include "framework/argument/abstract/enum_argument.h"
#include "framework/enum/buffer_contents.h"

struct BufferContentsArgument : EnumArgument<BufferContentsArgument, BufferContents> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "buffer contents";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[2] = {EnumType::Zeros, EnumType::Random};
    const static inline std::string enumValuesNames[2] = {"Zeros", "Random"};
};
