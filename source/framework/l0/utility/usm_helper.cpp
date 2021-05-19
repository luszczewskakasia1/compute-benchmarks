/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2021 Intel Corporation. All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to the
 * source code ("Material") are owned by Intel Corporation or its suppliers
 * or licensors. Title to the Material remains with Intel Corporation or its
 * suppliers and licensors. The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and licensors. The
 * Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 */

#include "framework/l0/utility/usm_helper.h"

namespace L0::UsmHelper {

ze_result_t allocate(UsmMemoryPlacement placement, LevelZero &levelZero, size_t size, void **buffer) {
    const ze_host_mem_alloc_desc_t hostAllocDesc{ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC};
    const ze_device_mem_alloc_desc_t deviceAllocDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
    switch (placement) {
    case UsmMemoryPlacement::Device:
        return zeMemAllocDevice(levelZero.context, &deviceAllocDesc, size, 0, levelZero.device, buffer);
    case UsmMemoryPlacement::Host:
        return zeMemAllocHost(levelZero.context, &hostAllocDesc, size, 0, buffer);
    case UsmMemoryPlacement::Shared:
        return zeMemAllocShared(levelZero.context, &deviceAllocDesc, &hostAllocDesc, size, 0, levelZero.device, buffer);
    case UsmMemoryPlacement::NonUsmImported: {
        *buffer = malloc(size);
        return levelZero.importHostPointer.importExternalPointer(levelZero.driver, *buffer, size);
    }
    case UsmMemoryPlacement::NonUsm: {
        *buffer = malloc(size);
        return ZE_RESULT_SUCCESS;
    }
    default:
        FATAL_ERROR("Unknown placement");
    }
}

ze_result_t deallocate(UsmMemoryPlacement placement, LevelZero &levelZero, void *buffer) {
    if (placement == UsmMemoryPlacement::NonUsm) {
        free(buffer);
        return ZE_RESULT_SUCCESS;
    } else if (placement == UsmMemoryPlacement::NonUsmImported) {
        auto ret = levelZero.importHostPointer.releaseExternalPointer(levelZero.driver, buffer);
        free(buffer);
        return ret;
    } else {
        return zeMemFree(levelZero.context, buffer);
    }
}

ze_result_t allocate(DeviceSelection placement, LevelZero &levelzero, size_t size, void **outBuffer) {
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
