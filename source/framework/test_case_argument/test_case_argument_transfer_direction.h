#pragma once

#include "framework/enum/transfer_direction.h"
#include "framework/test_case_argument/test_case_argument_enum.h"

struct TransferDirectionTestCaseArgument : EnumTestCaseArgument<TransferDirectionTestCaseArgument, TransferDirection> {
    using EnumTestCaseArgument::EnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "transfer direction";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[4] = {EnumType::SysToSys, EnumType::SysToDev, EnumType::DevToSys, EnumType::DevToDev};
    const static inline std::string enumValuesNames[4] = {"SysToSys", "SysToDev", "DevToSys", "DevToDev"};
};
