#pragma once

#include "framework/enum/device_selection.h"
#include "framework/test_case_argument/test_case_argument_bitfield_enum.h"

struct UsmDeviceSelectionTestCaseArgument : BitfieldEnumTestCaseArgument<UsmDeviceSelectionTestCaseArgument, DeviceSelection> {
    using BitfieldEnumTestCaseArgument::BitfieldEnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "usm device selection";
    const static inline EnumType zeroEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[6] = {EnumType::Host, EnumType::Root, EnumType::Tile0, EnumType::Tile1, EnumType::Tile2, EnumType::Tile3};
    const static inline std::string enumValuesNames[6] = {"Host", "Root", "Tile0", "Tile1", "Tile2", "Tile3"};

    bool validateExtra() const override {
        // Usm can have only one GPU device
        const auto gpuDevices = DeviceSelectionHelper::withoutHost(this->value);
        const auto gpuDevicesCount = DeviceSelectionHelper::getDevicesCount(gpuDevices);
        return gpuDevicesCount <= 1u;
    }
};
