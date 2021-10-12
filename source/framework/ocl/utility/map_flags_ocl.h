/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#pragma once

#include "framework/enum/map_flags.h"
#include "framework/ocl/opencl.h"

namespace OCL {
inline cl_map_flags convertMapFlags(MapFlags mapFlags) {
    switch (mapFlags) {
    case MapFlags::Read:
        return CL_MAP_READ;
    case MapFlags::Write:
        return CL_MAP_WRITE;
    case MapFlags::WriteInvalidate:
        return CL_MAP_WRITE_INVALIDATE_REGION;
    default:
        FATAL_ERROR("Unknown map flag");
    }
}
} // namespace OCL
