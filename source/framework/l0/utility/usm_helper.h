#pragma once

#include "framework/enum/device_selection.h"
#include "framework/enum/usm_memory_placement.h"
#include "framework/l0/levelzero.h"

namespace L0::UsmHelper {

inline ze_result_t allocate(UsmMemoryPlacement placement, ze_context_handle_t context, ze_device_handle_t device, size_t size, void **buffer) {
    const ze_host_mem_alloc_desc_t hostAllocDesc{ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC};
    const ze_device_mem_alloc_desc_t deviceAllocDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
    switch (placement) {
    case UsmMemoryPlacement::Device:
        return zeMemAllocDevice(context, &deviceAllocDesc, size, 0, device, buffer);
    case UsmMemoryPlacement::Host:
        return zeMemAllocHost(context, &hostAllocDesc, size, 0, buffer);
    case UsmMemoryPlacement::Shared:
        return zeMemAllocShared(context, &deviceAllocDesc, &hostAllocDesc, size, 0, device, buffer);
    case UsmMemoryPlacement::NonUsm: {
        *buffer = malloc(size);
        return ZE_RESULT_SUCCESS;
    }
    default:
        FATAL_ERROR("Unknown placement");
    }
}

inline ze_result_t allocate(DeviceSelection placement, LevelZero &levelzero, size_t size, void **outBuffer) {
    const DeviceSelection gpuDevice = DeviceSelectionHelper::withoutHost(placement);
    const auto gpuDevicesCount = DeviceSelectionHelper::getDevicesCount(gpuDevice);
    FATAL_ERROR_IF(gpuDevicesCount > 1, "USM allocations can have 0 or 1 gpu device");

    const bool hasDevice = gpuDevicesCount == 1;
    const bool hasHost = DeviceSelectionHelper::hasDevice(placement, DeviceSelection::Host);

    const ze_host_mem_alloc_desc_t hostAllocDesc{ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC};
    const ze_device_mem_alloc_desc_t deviceAllocDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};

    if (hasDevice && hasHost) {
        return zeMemAllocShared(levelzero.context, &deviceAllocDesc, &hostAllocDesc, size, 0, levelzero.getDevice(gpuDevice), outBuffer);
    }

    if (hasDevice) {
        return zeMemAllocDevice(levelzero.context, &deviceAllocDesc, size, 0, levelzero.getDevice(gpuDevice), outBuffer);
    }

    if (hasHost) {
        return zeMemAllocHost(levelzero.context, &hostAllocDesc, size, 0, outBuffer);
    }

    FATAL_ERROR("USM allocations need at least one storage location");
}

} // namespace L0::UsmHelper
