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

#include "framework/ocl/cl.h"
#include "framework/ocl/utility/error.h"
#include "framework/test_case/test_case.h"

namespace OCL::CompressionHelper {

inline cl_mem_flags getCompressionFlags(bool compression, bool noIntelExtensions) {
    if (noIntelExtensions) {
        return 0;
    }

    if (compression) {
        return CL_MEM_COMPRESSED_HINT_INTEL;
    } else {
        return CL_MEM_UNCOMPRESSED_HINT_INTEL;
    }
}

inline TestResult verifyCompression(cl_mem mem, bool expectedCompression, bool noIntelExtensions) {
    if (noIntelExtensions) {
        return TestResult::Success; // We have nothing to check, this is a vendor agnostic benchmark run
    }

    cl_bool isCompressedRaw{};
    const cl_int retVal = clGetMemObjectInfo(mem, CL_MEM_USES_COMPRESSION_INTEL, sizeof(isCompressedRaw), &isCompressedRaw, nullptr);
    if (retVal != CL_SUCCESS) {
        ASSERT_CL_SUCCESS(retVal); // We are not on an Intel GPU. Parameter --no-intel-extensions should be passed
    }

    const bool isCompressed = isCompressedRaw == CL_TRUE;
    if (isCompressed != expectedCompression) {
        return TestResult::DeviceNotCapable; // We are on an Intel GPU, which does not support compression, e.g. Skylake
    }
    return TestResult::Success;
}
} // namespace OCL::CompressionHelper
