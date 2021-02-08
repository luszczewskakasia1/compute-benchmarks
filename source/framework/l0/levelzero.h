#pragma once

#include "framework/configuration.h"
#include "framework/l0/context_properties.h"
#include "framework/l0/error.h"
#include "framework/l0/queue_properties.h"
#include "framework/utility/error.h"

#include <level_zero/ze_api.h>

namespace L0 {

// Class handles regular LevelZero boilerplate code, such as querying driver handle, devices, creating contexts,
// and queues. It is configurable by the QueueProperties and ContextProperties objects, allowing to perform the
// setup in a different way than usual. Default constructor always creates both the context and the queue.
//
// LevelZero performs it's own cleanup.
struct LevelZero {
    // Additional data of the command queue, which cannot be queried after creation
    struct QueueInfo {
        bool isCopyOnly = {};
        size_t maxFillSize = {};
        size_t count;
        ze_device_handle_t device = {};
        ze_command_queue_desc_t desc = {};
    };

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

    // Constructors, destructor
    LevelZero() : LevelZero(QueueProperties::create()) {}
    LevelZero(const QueueProperties &queueProperties) : LevelZero(queueProperties, ContextProperties::create()) {}
    LevelZero(const QueueProperties &queueProperties, const ContextProperties &contextProperties);
    ~LevelZero();

    // Queries available queue families on the device and returns their descriptions.
    static std::vector<QueueInfo> queryQueueFamilies(ze_device_handle_t device);

    // Creates queue with given properties. These methods aren't needed to be called by the user in scenarios with only one queue.
    // Queue will be created by default, unless disabled in QueueProperties. These methods allow the user to create
    // additional queues. Queues are tracked internally and will be released automatically.
    std::pair<ze_command_queue_handle_t, QueueInfo> createQueue(const QueueProperties &queueProperties);
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

  private:
    // Queriers subDevices of the root device and creates them if any. This method is only called when it's necessary, i.e. user
    // specified some subDevices in ContextProperties
    void createSubDevices(bool requireSuccess);

    // Internal fields managed by the LevelZero class
    ze_device_handle_t rootDevice{};
    std::vector<ze_device_handle_t> subDevices{};
    std::vector<ze_command_queue_handle_t> commandQueues{};
};
} // namespace L0

using namespace L0;
