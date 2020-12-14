#pragma once

#include "framework/ocl/cl.h"
#include "framework/utility/string_utils.h"

#include <algorithm>
#include <memory>

namespace OCL {
class ExtensionsHelper {
  public:
    ExtensionsHelper(cl_device_id device) : extensions(queryExtensions(device)) {}

    bool isSupported(const char *extension, bool allowPreview = false) const {
        const auto previewExtension = std::string(extension) + "_preview";
        const auto predicate = [&](const std::string &currentExtension) {
            return currentExtension == extension || (allowPreview && currentExtension == previewExtension);
        };
        return std::any_of(extensions.begin(), extensions.end(), predicate);
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

    const std::vector<std::string> extensions;
};
} // namespace OCL
