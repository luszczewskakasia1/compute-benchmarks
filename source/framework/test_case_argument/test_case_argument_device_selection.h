#pragma once

#include "framework/enum/device_selection.h"
#include "framework/test_case_argument/test_case_argument_enum.h"

struct DeviceSelectionTestCaseArgument : EnumTestCaseArgument<DeviceSelectionTestCaseArgument, DeviceSelection> {
    using EnumTestCaseArgument::EnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "device selection";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[5] = {EnumType::Root, EnumType::Tile0, EnumType::Tile1, EnumType::Tile2, EnumType::Tile3};
    const static inline std::string enumValuesNames[5] = {"Root", "Tile0", "Tile1", "Tile2", "Tile3"};
};
