#pragma once

#include "framework/utility/error.h"

enum class DeviceSelection : int {
    Unknown = 0,
    Host = 1 << 0, // For USM
    Root = 1 << 1,
    Tile0 = 1 << 2,
    Tile1 = 1 << 3,
    Tile2 = 1 << 4,
    Tile3 = 1 << 5,
};

inline DeviceSelection operator|(DeviceSelection a, DeviceSelection b) {
    return static_cast<DeviceSelection>(static_cast<int>(a) | static_cast<int>(b));
}

inline DeviceSelection operator&(DeviceSelection a, DeviceSelection b) {
    return static_cast<DeviceSelection>(static_cast<int>(a) & static_cast<int>(b));
}

inline DeviceSelection operator~(DeviceSelection a) {
    return static_cast<DeviceSelection>(~static_cast<int>(a));
}

struct DeviceSelectionHelper {
    const static inline DeviceSelection devices[] = {DeviceSelection::Host, DeviceSelection::Root, DeviceSelection::Tile0, DeviceSelection::Tile1, DeviceSelection::Tile2, DeviceSelection::Tile3};
    const static inline DeviceSelection subDevices[] = {DeviceSelection::Tile0, DeviceSelection::Tile1, DeviceSelection::Tile2, DeviceSelection::Tile3};

    static size_t getSubDeviceIndex(DeviceSelection deviceSelection) {
        switch (deviceSelection) {
        case DeviceSelection::Tile0:
            return 0;
        case DeviceSelection::Tile1:
            return 1;
        case DeviceSelection::Tile2:
            return 2;
        case DeviceSelection::Tile3:
            return 3;
        default:
            ERROR("Unknown device selection");
        }
    }

    static bool hasDevice(DeviceSelection deviceSelection, DeviceSelection device) {
        ERROR_IF(device == DeviceSelection::Unknown, "Cannot check for unknown device");
        return ((deviceSelection & device) == device);
    }

    static bool hasAnySubDevice(DeviceSelection deviceSelection) {
        for (auto subDevice : subDevices) {
            if (hasDevice(deviceSelection, subDevice)) {
                return true;
            }
        }
        return false;
    }

    static size_t getDevicesCount(DeviceSelection deviceSelection) {
        size_t deviceCount = 0u;
        for (auto device : devices) {
            if (hasDevice(deviceSelection, device)) {
                deviceCount++;
            }
        }
        return deviceCount;
    }

    static bool hasSingleDevice(DeviceSelection deviceSelection) {
        return getDevicesCount(deviceSelection) == 1u;
    }

    static bool hasHost(DeviceSelection deviceSelection) {
        return hasDevice(deviceSelection, DeviceSelection::Host);
    }

    static DeviceSelection withoutHost(DeviceSelection deviceSelection) {
        return deviceSelection & ~DeviceSelection::Host;
    }

    static bool isSubset(DeviceSelection superset, DeviceSelection subset) {
        for (auto device : devices) {
            if (hasDevice(subset, device) && !hasDevice(superset, device)) {
                return false;
            }
        }
        return true;
    }
};
