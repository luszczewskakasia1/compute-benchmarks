#pragma once

#include "framework/enum/memory_placement.h"
#include "framework/ocl/opencl.h"

inline void *clHostOrDeviceOrSharedAllocINTEL(MemoryPlacement placement, cl_platform_id platform, cl_context context,
                                              cl_device_id device, size_t bufferSize, cl_int *retVal) {
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
