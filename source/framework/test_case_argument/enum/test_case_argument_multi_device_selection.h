#pragma once

#include "framework/enum/device_selection.h"
#include "framework/test_case_argument/abstract/test_case_argument_bitfield_enum.h"

struct MultiDeviceSelectionTestCaseArgument : BitfieldEnumTestCaseArgument<MultiDeviceSelectionTestCaseArgument, DeviceSelection> {
    using BitfieldEnumTestCaseArgument::BitfieldEnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "multi device selection";
    const static inline EnumType zeroEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[5] = {EnumType::Root, EnumType::Tile0, EnumType::Tile1, EnumType::Tile2, EnumType::Tile3};
    const static inline std::string enumValuesNames[5] = {"Root", "Tile0", "Tile1", "Tile2", "Tile3"};
};

struct MultipleTilesSelectionTestCaseArgument : BitfieldEnumTestCaseArgument<MultipleTilesSelectionTestCaseArgument, DeviceSelection> {
    using BitfieldEnumTestCaseArgument::BitfieldEnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "multiple tiles selection";
    const static inline EnumType zeroEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[4] = {EnumType::Tile0, EnumType::Tile1, EnumType::Tile2, EnumType::Tile3};
    const static inline std::string enumValuesNames[4] = {"Tile0", "Tile1", "Tile2", "Tile3"};
};
