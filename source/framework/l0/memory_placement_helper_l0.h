#pragma once

#include "framework/enum/memory_placement.h"
#include "framework/l0/levelzero.h"

inline ze_result_t zeMemAllocHostOrDeviceOrShared(MemoryPlacement placement, ze_context_handle_t context,
                                                  ze_device_handle_t device, size_t size, void **buffer) {
    const ze_host_mem_alloc_desc_t hostAllocDesc{ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC};
    const ze_device_mem_alloc_desc_t deviceAllocDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
    switch (placement) {
    case MemoryPlacement::Device:
        return zeMemAllocDevice(context, &deviceAllocDesc, size, 0, device, buffer);
    case MemoryPlacement::Host:
        return zeMemAllocHost(context, &hostAllocDesc, size, 0, buffer);
    case MemoryPlacement::Shared:
        return zeMemAllocShared(context, &deviceAllocDesc, &hostAllocDesc, size, 0, device, buffer);
    default:
        ERROR("Unknown placement");
    }
}
