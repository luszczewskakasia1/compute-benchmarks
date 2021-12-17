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
#include "framework/utility/string_utils.h"

#include <algorithm>
#include <memory>

struct UsmFunctions {
    pfn_clMemFreeINTEL clMemFreeINTEL{};
    pfn_clHostMemAllocINTEL clHostMemAllocINTEL{};
    pfn_clDeviceMemAllocINTEL clDeviceMemAllocINTEL{};
    pfn_clSharedMemAllocINTEL clSharedMemAllocINTEL{};
};

namespace OCL {
class ExtensionsHelper {
  public:
    ExtensionsHelper(cl_device_id device) : extensions(queryExtensions(device)) {
        EXPECT_CL_SUCCESS(clGetDeviceInfo(device, CL_DEVICE_PLATFORM, sizeof(platform), &platform, nullptr));
    }

    bool isSupported(const char *extension, bool allowPreview = false) const {
        const auto previewExtension = std::string(extension) + "_preview";
        const bool allowIntelExtensions = !Configuration::get().noIntelExtensions;
        const auto predicate = [&](const std::string &currentExtension) {
            const bool isMatched = currentExtension == extension || (allowPreview && currentExtension == previewExtension);
            const bool isIntel = currentExtension.find("cl_intel_") == 0u;
            return isMatched && (allowIntelExtensions || !isIntel);
        };
        return std::any_of(extensions.begin(), extensions.end(), predicate);
    }

    bool areDoublesSupported() const {
        return isSupported("cl_khr_fp64");
    }

    bool isGlobalFloatAtomicsSupported() const {
        return isSupported("cl_intel_global_float_atomics");
    }

    bool isCommandQueueFamiliesSupported() const {
        return isSupported("cl_intel_command_queue_families", true);
    }

    bool isUsmSupported() const {
        return isSupported("cl_intel_unified_shared_memory", true);
    }

    UsmFunctions queryUsmFunctions() const {
        UsmFunctions result{};
        if (isUsmSupported()) {
            result.clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clMemFreeINTEL");
            result.clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clHostMemAllocINTEL");
            result.clDeviceMemAllocINTEL = (pfn_clDeviceMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clDeviceMemAllocINTEL");
            result.clSharedMemAllocINTEL = (pfn_clSharedMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clSharedMemAllocINTEL");
        }
        return result;
    }

  private:
    static std::vector<std::string> queryExtensions(cl_device_id device) {
        size_t infoSize{};
        if (CL_SUCCESS != clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, nullptr, &infoSize)) {
            return {};
        }

        auto extensionString = std::make_unique<char[]>(infoSize);
        if (CL_SUCCESS != clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, infoSize, extensionString.get(), nullptr)) {
            return {};
        }

        return splitString(extensionString.get());
    }

    cl_platform_id platform{};
    const std::vector<std::string> extensions{};
};
} // namespace OCL
