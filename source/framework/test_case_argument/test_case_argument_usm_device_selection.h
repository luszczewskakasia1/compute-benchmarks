#pragma once

#include "framework/enum/device_selection.h"
#include "framework/test_case_argument/abstract/test_case_argument_bitfield_enum.h"

struct DeviceSelectionTestCaseArgumentBaseTraits {
    using EnumType = DeviceSelection;
    const static inline std::string enumName = "usm device selection";
    const static inline EnumType zeroEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[6] = {EnumType::Host, EnumType::Root, EnumType::Tile0, EnumType::Tile1, EnumType::Tile2, EnumType::Tile3};
    const static inline std::string enumValuesNames[6] = {"Host", "Root", "Tile0", "Tile1", "Tile2", "Tile3"};
};

template <bool allowHost, bool allowDevice, bool allowShared>
struct UsmDeviceSelectionTestCaseArgumentBase : BitfieldEnumTestCaseArgument<DeviceSelectionTestCaseArgumentBaseTraits, DeviceSelection> {
    using BitfieldEnumTestCaseArgument::BitfieldEnumTestCaseArgument;

    UsmDeviceSelectionTestCaseArgumentBase &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    bool validateExtra() const override {
        // Usm can have only one GPU device
        const auto gpuDevices = DeviceSelectionHelper::withoutHost(this->value);
        const auto gpuDevicesCount = DeviceSelectionHelper::getDevicesCount(gpuDevices);
        if (gpuDevicesCount > 1) {
            return false;
        }

        const bool hasHost = DeviceSelectionHelper::hasDevice(this->value, DeviceSelection::Host);
        const bool hasDevice = gpuDevicesCount == 1;
        if (hasHost && hasDevice) {
            return allowShared;
        }

        if (hasHost) {
            return allowHost;
        }

        if (hasDevice) {
            return allowDevice;
        }

        ERROR("Unreachable code in UsmDeviceSelectionTestCaseArgumentBase");
    }
};

using UsmDeviceSelectionTestCaseArgument = UsmDeviceSelectionTestCaseArgumentBase<true, true, true>;
using UsmSharedDeviceSelectionTestCaseArgument = UsmDeviceSelectionTestCaseArgumentBase<false, false, true>;
