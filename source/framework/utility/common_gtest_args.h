#pragma once

#include "framework/enum/api.h"
#include "framework/enum/atomic_operation.h"
#include "framework/enum/device_selection.h"

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

inline auto usmDeviceSelections() {
    return ::testing::Values(DeviceSelection::Host,                           // Host
                             DeviceSelection::Root,                           // Device
                             DeviceSelection::Tile0,                          // Device
                             DeviceSelection::Tile1,                          // Device
                             DeviceSelection::Host | DeviceSelection::Root,   // Shared
                             DeviceSelection::Host | DeviceSelection::Tile0,  // Shared
                             DeviceSelection::Host | DeviceSelection::Tile1); // Shared
}

inline auto usmSharedSelections() {
    return ::testing::Values(DeviceSelection::Host | DeviceSelection::Root,   // Shared
                             DeviceSelection::Host | DeviceSelection::Tile0,  // Shared
                             DeviceSelection::Host | DeviceSelection::Tile1); // Shared
}

inline auto allAtomicOperations() {
    return ::testing::Values(AtomicOperation::Add, AtomicOperation::Sub,
                             AtomicOperation::Xchg, AtomicOperation::CmpXchg,
                             AtomicOperation::Inc, AtomicOperation::Dec,
                             AtomicOperation::Min, AtomicOperation::Max,
                             AtomicOperation::And, AtomicOperation::Or,
                             AtomicOperation::Xor);
}

} // namespace CommonGtestArgs
