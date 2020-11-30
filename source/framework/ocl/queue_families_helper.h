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
        cl_uint numFamilies{};
        cl_int retVal = clGetDeviceInfo(device, CL_DEVICE_NUM_QUEUE_FAMILIES_INTEL, sizeof(numFamilies), &numFamilies, nullptr);
        if (retVal != CL_SUCCESS || numFamilies == 0) {
            return {};
        }

        auto families = std::make_unique<cl_queue_family_properties_intel[]>(numFamilies);
        retVal = clGetDeviceInfo(device, CL_DEVICE_QUEUE_FAMILY_PROPERTIES_INTEL, sizeof(families[0]) * numFamilies, families.get(), nullptr);
        if (retVal != CL_SUCCESS) {
            return {};
        }

        std::vector<QueueFamilyDesc> result = {};
        for (auto familyIndex = 0u; familyIndex < numFamilies; familyIndex++) {
            QueueFamilyDesc desc{};
            desc.properties.properties[0] = CL_QUEUE_FAMILY_INTEL;
            desc.properties.properties[1] = familyIndex;
            desc.properties.properties[2] = CL_QUEUE_INDEX_INTEL;
            desc.properties.properties[3] = 0;
            desc.properties.propertiesCount = 4;
            desc.queueCount = families[familyIndex].count;
            desc.copyOnly = (families[familyIndex].capabilities & CL_QUEUE_CAPABILITY_KERNEL_INTEL) == 0;

            result.push_back(desc);
        }

        return result;
    }
};
