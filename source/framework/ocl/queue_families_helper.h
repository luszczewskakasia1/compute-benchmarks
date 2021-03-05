#pragma once

#include "framework/ocl/cl.h"

#include <memory>
#include <vector>

class QueueFamiliesHelper {
  public:
    struct PropertiesForSelectingQueue {
        cl_queue_properties properties[4];
        cl_uint propertiesCount;
    };

    static inline std::unique_ptr<PropertiesForSelectingQueue> getPropertiesForSelectingBlitter(cl_device_id device, bool useLegacy) {
        if (useLegacy) {
            return getPropertiesForSelectingBlitterLegacy();
        } else {
            return getPropertiesForSelectingBlitterNew(device);
        }
    }

    static inline std::unique_ptr<PropertiesForSelectingQueue> getPropertiesForSelectingBlitterNew(cl_device_id device) {
        for (const auto &queueFamilyDesc : queryQueueFamilies(device)) {
            if (queueFamilyDesc.copyOnly) {
                return std::make_unique<PropertiesForSelectingQueue>(queueFamilyDesc.properties);
            }
        }
        return nullptr;
    }

    static inline std::unique_ptr<PropertiesForSelectingQueue> getPropertiesForSelectingBlitterLegacy() {
        auto result = std::make_unique<PropertiesForSelectingQueue>();
        result->properties[0] = CL_QUEUE_FAMILY_DEPRECATED_INTEL;
        result->properties[1] = CL_QUEUE_FAMILY_TYPE_BCS_DEPRECATED_INTEL;
        result->propertiesCount = 2;
        return result;
    }

  private:
    struct QueueFamilyDesc {
        PropertiesForSelectingQueue properties;
        size_t queueCount;
        bool copyOnly;
    };

    static inline std::vector<QueueFamilyDesc> queryQueueFamilies(cl_device_id device) {
        // Get families count
        size_t familyPropertiesSize{};
        cl_int retVal = clGetDeviceInfo(device, CL_DEVICE_QUEUE_FAMILY_PROPERTIES_INTEL, 0, nullptr, &familyPropertiesSize);
        if (retVal != CL_SUCCESS) {
            return {};
        }
        size_t familiesCount = familyPropertiesSize / sizeof(cl_queue_family_properties_intel);
        if (familiesCount == 0 || familiesCount * sizeof(cl_queue_family_properties_intel) != familyPropertiesSize) {
            return {};
        }

        // Get families
        auto families = std::make_unique<cl_queue_family_properties_intel[]>(familiesCount);
        retVal = clGetDeviceInfo(device, CL_DEVICE_QUEUE_FAMILY_PROPERTIES_INTEL, familyPropertiesSize, families.get(), nullptr);
        if (retVal != CL_SUCCESS) {
            return {};
        }

        std::vector<QueueFamilyDesc> result = {};
        for (auto familyIndex = 0u; familyIndex < familiesCount; familyIndex++) {
            QueueFamilyDesc desc{};
            desc.properties.properties[0] = CL_QUEUE_FAMILY_INTEL;
            desc.properties.properties[1] = familyIndex;
            desc.properties.properties[2] = CL_QUEUE_INDEX_INTEL;
            desc.properties.properties[3] = 0;
            desc.properties.propertiesCount = 4;
            desc.queueCount = families[familyIndex].count;
            desc.copyOnly = !validateCapability(families[familyIndex].capabilities, CL_QUEUE_CAPABILITY_KERNEL_INTEL);

            result.push_back(desc);
        }

        return result;
    }

    static bool validateCapability(cl_command_queue_capabilities_intel queueCapabilities, cl_command_queue_capabilities_intel capability) {
        return queueCapabilities == CL_QUEUE_DEFAULT_CAPABILITIES_INTEL || ((queueCapabilities & capability) == capability);
    }
};
