#pragma once

#include "framework/enum/device_selection.h"
#include "framework/enum/memory_placement.h"
#include "framework/ocl/opencl.h"

namespace UsmHelper {
inline bool supportsUsm(cl_platform_id platform) {
    const char *functions[] = {
        "clHostMemAllocINTEL",
        "clDeviceMemAllocINTEL",
        "clSharedMemAllocINTEL",
        "clMemFreeINTEL",
        "clEnqueueMemFillINTEL",
        "clEnqueueMemsetINTEL",
        "clEnqueueMemcpyINTEL",
    };

    for (const auto function : functions) {
        if (clGetExtensionFunctionAddressForPlatform(platform, function) == nullptr) {
            return false;
        }
    }
    return true;
}

inline void *allocate(MemoryPlacement placement, cl_platform_id platform, cl_context context, cl_device_id device, size_t bufferSize, cl_int *retVal) {
    switch (placement) {
    case MemoryPlacement::Device: {
        auto clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clDeviceMemAllocINTEL");
        return clDeviceMemAllocINTEL(context, device, nullptr, bufferSize, 0, retVal);
    }
    case MemoryPlacement::Host: {
        auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clHostMemAllocINTEL");
        return clHostMemAllocINTEL(context, nullptr, bufferSize, 0, retVal);
    }
    case MemoryPlacement::Shared: {
        auto clSharedMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clSharedMemAllocINTEL");
        return clSharedMemAllocINTEL(context, device, nullptr, bufferSize, 0, retVal);
    }
    default:
        ERROR("Unknown placement");
    }
}

inline void *allocate(DeviceSelection placement, Opencl &opencl, size_t bufferSize, cl_int *retVal) {
    const DeviceSelection gpuDevice = DeviceSelectionHelper::withoutHost(placement);
    const auto gpuDevicesCount = DeviceSelectionHelper::getDevicesCount(gpuDevice);
    ERROR_IF(gpuDevicesCount > 1, "USM allocations can have 0 or 1 gpu device");

    const bool hasDevice = gpuDevicesCount == 1;
    const bool hasHost = DeviceSelectionHelper::hasDevice(placement, DeviceSelection::Host);

    if (hasDevice && hasHost) {
        auto clSharedMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clSharedMemAllocINTEL");
        return clSharedMemAllocINTEL(opencl.context, opencl.getDevice(gpuDevice), nullptr, bufferSize, 0, retVal);
    }

    if (hasDevice) {
        auto clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clDeviceMemAllocINTEL");
        return clDeviceMemAllocINTEL(opencl.context, opencl.getDevice(gpuDevice), nullptr, bufferSize, 0, retVal);
    }

    if (hasHost) {
        auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(opencl.platform, "clHostMemAllocINTEL");
        return clHostMemAllocINTEL(opencl.context, nullptr, bufferSize, 0, retVal);
    }

    ERROR("USM allocations need at least one storage location");
}

} // namespace UsmHelper
