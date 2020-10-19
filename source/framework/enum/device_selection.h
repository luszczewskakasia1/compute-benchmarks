#pragma once

enum class DeviceSelection {
    Unknown = 0,
    Root = 1 << 1,
    Tile0 = 1 << 2,
    Tile1 = 1 << 3,
    Tile2 = 1 << 4,
    Tile3 = 1 << 5,
};

inline size_t getSubDeviceIndexFromDeviceSelection(DeviceSelection deviceSelection) {
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
