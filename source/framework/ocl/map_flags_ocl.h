#pragma once

#include "framework/map_flags.h"
#include "framework/ocl/opencl.h"

inline cl_map_flags convertMapFlags(MapFlags mapFlags) {
    switch (mapFlags) {
    case MapFlags::Read:
        return CL_MAP_READ;
    case MapFlags::Write:
        return CL_MAP_WRITE;
    case MapFlags::WriteInvalidate:
        return CL_MAP_WRITE_INVALIDATE_REGION;
    default:
        ERROR("Unknown map flag");
    }
}
