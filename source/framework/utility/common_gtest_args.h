#include <gtest/gtest.h>

namespace CommonGtestArgs {
inline auto allApis() {
    return ::testing::Values(Api::OpenCL, Api::L0);
}

inline auto workgroupCount() {
    return ::testing::Values(1, 100, 1000, 10000);
}

inline auto workgroupSize() {
    return ::testing::Values(1, 16, 32, 256);
}

inline auto contextDeviceSelections() {
    return ::testing::Values(DeviceSelection::Root,                                                    // 1. Naive
                             DeviceSelection::Tile0,                                                   // 2. Ninja
                             DeviceSelection::Tile1,                                                   // 3. Ninja
                             DeviceSelection::Tile0 | DeviceSelection::Tile1,                          // 4. Ninja
                             DeviceSelection::Root | DeviceSelection::Tile0 | DeviceSelection::Tile1); // 5. Convenience
}

inline auto resourceDeviceSelections() {
    return ::testing::Values(DeviceSelection::Root,
                             DeviceSelection::Tile0,
                             DeviceSelection::Tile1);
}

} // namespace CommonGtestArgs
