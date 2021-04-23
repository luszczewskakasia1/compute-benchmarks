#pragma once

#include "framework/enum/device_selection.h"
#include "framework/enum/usm_initial_placement.h"
#include "framework/enum/usm_memory_placement.h"
#include "framework/ocl/opencl.h"

namespace OCL::UsmHelper {
inline void *allocate(UsmMemoryPlacement placement, cl_platform_id platform, cl_context context, cl_device_id device, size_t bufferSize, cl_int *retVal) {
    switch (placement) {
    case UsmMemoryPlacement::Device: {
        auto clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clDeviceMemAllocINTEL");
        return clDeviceMemAllocINTEL(context, device, nullptr, bufferSize, 0, retVal);
    }
    case UsmMemoryPlacement::Host: {
        auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clHostMemAllocINTEL");
        return clHostMemAllocINTEL(context, nullptr, bufferSize, 0, retVal);
    }
    case UsmMemoryPlacement::Shared: {
        auto clSharedMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clSharedMemAllocINTEL");
        return clSharedMemAllocINTEL(context, device, nullptr, bufferSize, 0, retVal);
    }
    case UsmMemoryPlacement::NonUsm: {
        return malloc(bufferSize);
    }

    default:
        FATAL_ERROR("Unknown placement");
    }
}

inline void *allocate(DeviceSelection placement, Opencl &opencl, size_t bufferSize, cl_int *retVal) {
    const DeviceSelection gpuDevice = DeviceSelectionHelper::withoutHost(placement);
    const auto gpuDevicesCount = DeviceSelectionHelper::getDevicesCount(gpuDevice);
    FATAL_ERROR_IF(gpuDevicesCount > 1, "USM allocations can have 0 or 1 gpu device");

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

    FATAL_ERROR("USM allocations need at least one storage location");
}

inline cl_mem_properties_intel getInitialPlacementFlag(UsmInitialPlacement placement) {
    switch (placement) {
    case UsmInitialPlacement::Any:
        return 0;
    case UsmInitialPlacement::Host:
        return CL_MEM_ALLOC_INITIAL_PLACEMENT_HOST_INTEL;
    case UsmInitialPlacement::Device:
        return CL_MEM_ALLOC_INITIAL_PLACEMENT_DEVICE_INTEL;
    default:
        FATAL_ERROR("Unknown USM initial placement");
    }
}

} // namespace OCL::UsmHelper
