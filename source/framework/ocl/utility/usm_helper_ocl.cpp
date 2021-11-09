#include "usm_helper_ocl.h"

cl_int UsmHelperOcl::allocate(Opencl &opencl,
                              UsmMemoryPlacement placement,
                              size_t bufferSize,
                              Alloc &outAlloc) {
    outAlloc.placement = placement;
    outAlloc.usm = opencl.getExtensions().queryUsmFunctions();
    outAlloc.context = opencl.context;

    cl_int retVal = CL_SUCCESS;

    switch (placement) {
    case UsmMemoryPlacement::Device:
        outAlloc.ptr = outAlloc.usm.clDeviceMemAllocINTEL(opencl.context, opencl.device, nullptr, bufferSize, 0, &retVal);
        break;
    case UsmMemoryPlacement::Host:
        outAlloc.ptr = outAlloc.usm.clHostMemAllocINTEL(opencl.context, nullptr, bufferSize, 0, &retVal);
        break;
    case UsmMemoryPlacement::Shared:
        outAlloc.ptr = outAlloc.usm.clSharedMemAllocINTEL(opencl.context, opencl.device, nullptr, bufferSize, 0, &retVal);
        break;
    case UsmMemoryPlacement::NonUsm:
        outAlloc.ptr = new uint8_t[bufferSize];
        break;
    default:
        FATAL_ERROR("Unknown placement");
    }

    return retVal;
}

cl_int UsmHelperOcl::deallocate(Alloc &alloc) {

    cl_int retVal = CL_SUCCESS;

    switch (alloc.placement) {
    case UsmMemoryPlacement::Device:
    case UsmMemoryPlacement::Host:
    case UsmMemoryPlacement::Shared:
        retVal = alloc.usm.clMemFreeINTEL(alloc.context, alloc.ptr);
        break;
    case UsmMemoryPlacement::NonUsm:
        delete[](static_cast<uint8_t *>(alloc.ptr));
        break;
    default:
        FATAL_ERROR("Unknown placement");
    }

    alloc = {};

    return retVal;
}

void *UsmHelperOcl::allocate(DeviceSelection placement, Opencl &opencl, size_t bufferSize, cl_int *retVal) {
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

cl_mem_properties_intel UsmHelperOcl::getInitialPlacementFlag(UsmInitialPlacement placement) {
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
