/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2021 Intel Corporation. All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to the
 * source code ("Material") are owned by Intel Corporation or its suppliers
 * or licensors. Title to the Material remains with Intel Corporation or its
 * suppliers and licensors. The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and licensors. The
 * Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 */

#pragma once

#include "framework/configuration.h"
#include "framework/l0/context_properties.h"
#include "framework/l0/extension_properties.h"
#include "framework/l0/queue_properties.h"
#include "framework/l0/utility/error.h"
#include "framework/l0/utility/queue_families_helper.h"

#include <level_zero/ze_api.h>

namespace L0 {
struct ImportHostPointerExtension {
    L0ImportExternalPointer importExternalPointer = nullptr;
    L0ReleaseImportedPointer releaseExternalPointer = nullptr;
    L0GetHostPointerBaseAddress getHostPointerBaseAddress = nullptr;
};

// Class handles regular LevelZero boilerplate code, such as querying driver handle, devices, creating contexts,
// and queues. It is configurable by the QueueProperties and ContextProperties objects, allowing to perform the
// setup in a different way than usual. Default constructor always creates both the context and the queue.
//
// LevelZero performs it's own cleanup.
struct LevelZero {
    // Public fields, accessible in benchmarks
    const size_t driverIndex;
    const size_t rootDeviceIndex;
    ze_driver_handle_t driver{};              // Driver instance, always present
    ze_device_handle_t device{};              // Default device, it is not present if multiple devices were selected in ContextSelection
    ze_context_handle_t context{};            // Context, created by default, can be disabled
    ze_command_queue_handle_t commandQueue{}; // Queue, created by default, can be disabled.
    ze_command_queue_desc_t commandQueueDesc{};
    ze_device_handle_t commandQueueDevice{};
    size_t commandQueueMaxFillSize{};
    ImportHostPointerExtension importHostPointer{};

    // Constructors, destructor
    LevelZero() : LevelZero(QueueProperties::create()) {}
    LevelZero(const QueueProperties &queueProperties) : LevelZero(queueProperties, ContextProperties::create()) {}
    LevelZero(const QueueProperties &queueProperties, const ContextProperties &contextProperties)
        : LevelZero(queueProperties, contextProperties, ExtensionProperties::create()) {}
    LevelZero(const QueueProperties &queueProperties, const ContextProperties &contextProperties,
              const ExtensionProperties &extensionProperties);
    ~LevelZero();

    // Returns how many subDevices has been created. Will return 0, if no subDevices were specified in ContextProperties or
    // QueueProperties.
    size_t getSubDevicesCount() const { return subDevices.size(); }

    // Create LevelZero context
    ze_context_handle_t createContext(const ContextProperties &contextProperties);

    // Creates queue with given properties. These methods aren't needed to be called by the user in scenarios with only one queue.
    // Queue will be created by default, unless disabled in QueueProperties. These methods allow the user to create
    // additional queues. Queues are tracked internally and will be released automatically.
    QueueFamiliesHelper::QueueDesc createQueue(const QueueProperties &queueProperties);
    ze_command_queue_handle_t createQueue(ze_device_handle_t device, ze_command_queue_desc_t desc);

    // Returns device for given DeviceSelection. Getting multiple devices at once, e.g. Tile0|Tile1 is forbidden.
    ze_device_handle_t getDevice(DeviceSelection deviceSelection) const;

    // Utility methods for L0 getter functions
    ze_driver_ipc_properties_t getIpcProperties() const {
        ze_driver_ipc_properties_t ipcProperties{ZE_STRUCTURE_TYPE_DRIVER_IPC_PROPERTIES};
        EXPECT_ZE_RESULT_SUCCESS(zeDriverGetIpcProperties(driver, &ipcProperties));
        return ipcProperties;
    }
    ze_device_properties_t getDeviceProperties() const { return getDeviceProperties(this->device); }
    ze_device_properties_t getDeviceProperties(DeviceSelection deviceSelection) const { return getDeviceProperties(getDevice(deviceSelection)); }
    ze_device_properties_t getDeviceProperties(ze_device_handle_t device) const {
        ze_device_properties_t deviceProperties{ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetProperties(device, &deviceProperties));
        return deviceProperties;
    }
    uint64_t getTimerResoultion(DeviceSelection deviceSelection) const { return getDeviceProperties(deviceSelection).timerResolution; }
    uint64_t getTimerResoultion(ze_device_handle_t device) const { return getDeviceProperties(device).timerResolution; }

    void initializeImportHostPointerExtension(const ExtensionProperties &extensionProperties);

  private:
    // Queriers subDevices of the root device and creates them if any. This method is only called when it's necessary, i.e. user
    // specified some subDevices in ContextProperties
    void createSubDevices(bool requireSuccess, bool fakeSubDeviceAllowed);

    // Internal fields managed by the LevelZero class
    ze_device_handle_t rootDevice{};
    std::vector<ze_device_handle_t> subDevices{};
    std::vector<ze_command_queue_handle_t> commandQueues{};
};
} // namespace L0

using namespace L0;
