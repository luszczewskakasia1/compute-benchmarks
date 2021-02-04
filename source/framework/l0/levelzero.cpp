#include "levelzero.h"

namespace L0 {
LevelZero::LevelZero(const QueueProperties &queueProperties, const ContextProperties &contextProperties) {
    EXPECT_ZE_RESULT_SUCCESS(zeInit(ZE_INIT_FLAG_GPU_ONLY));

    // Get driver
    uint32_t driverCount = 0;
    EXPECT_ZE_RESULT_SUCCESS(zeDriverGet(&driverCount, nullptr));
    const auto driverIndex = Configuration::get().l0DriverIndex;
    if (driverIndex >= driverCount) {
        FATAL_ERROR("Invalid LevelZero driver selected");
    }
    auto drivers = std::make_unique<ze_driver_handle_t[]>(driverCount);
    EXPECT_ZE_RESULT_SUCCESS(zeDriverGet(&driverCount, drivers.get()));
    this->driver = drivers[driverIndex];

    // Create root device
    uint32_t deviceCount = 0;
    EXPECT_ZE_RESULT_SUCCESS(zeDeviceGet(driver, &deviceCount, nullptr));
    const auto deviceIndex = Configuration::get().l0DeviceIndex;
    if (deviceIndex >= deviceCount) {
        FATAL_ERROR("Invalid LevelZero device selected");
    }
    auto devices = std::make_unique<ze_device_handle_t[]>(deviceCount);
    EXPECT_ZE_RESULT_SUCCESS(zeDeviceGet(driver, &deviceCount, devices.get()));
    this->rootDevice = devices[deviceIndex];

    // Create subDevices if needed
    if (DeviceSelectionHelper::hasAnySubDevice(contextProperties.deviceSelection)) {
        this->createSubDevices(contextProperties.requireCreationSuccess);
        if (this->subDevices.size() == 0) {
            return;
        }
    }

    // Set the default device
    if (DeviceSelectionHelper::hasSingleDevice(contextProperties.deviceSelection)) {
        this->device = getDevice(contextProperties.deviceSelection);
    }

    // Create context
    const ze_context_desc_t contextDesc{ZE_STRUCTURE_TYPE_CONTEXT_DESC};
    EXPECT_ZE_RESULT_SUCCESS(zeContextCreate(driver, &contextDesc, &context));

    // Create queue
    const auto [queue, queueInfo] = createQueue(queueProperties);
    this->commandQueue = queue;
    this->commandQueueDesc = queueInfo.desc;
    this->commandQueueDevice = queueInfo.device;
    this->commandQueueMaxFillSize = queueInfo.maxFillSize;
}

LevelZero::~LevelZero() {
    for (auto &queue : commandQueues) {
        EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueDestroy(queue));
    }
    if (context != nullptr) {
        EXPECT_ZE_RESULT_SUCCESS(zeContextDestroy(context));
    }
}

ze_device_handle_t LevelZero::getDevice(DeviceSelection deviceSelection) const {
    FATAL_ERROR_IF(DeviceSelectionHelper::hasHost(deviceSelection), "Cannot get ze_device_handle_t for host");
    FATAL_ERROR_UNLESS(DeviceSelectionHelper::hasSingleDevice(deviceSelection), "Cannot get multiple devices");
    if (deviceSelection == DeviceSelection::Root) {
        return this->rootDevice;
    }

    const auto subDeviceIndex = DeviceSelectionHelper::getSubDeviceIndex(deviceSelection);
    FATAL_ERROR_UNLESS((subDeviceIndex < this->subDevices.size()), "Invalid subDevice index");
    return this->subDevices[subDeviceIndex];
}

void LevelZero::createSubDevices(bool requireSuccess) {
    uint32_t numSubDevices{};
    EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetSubDevices(this->rootDevice, &numSubDevices, nullptr));
    if (numSubDevices == 0) {
        FATAL_ERROR_IF(requireSuccess, "SubDevice was selected, but device has 0 subDevices");
        return;
    }

    subDevices.resize(numSubDevices);
    EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetSubDevices(this->rootDevice, &numSubDevices, subDevices.data()));
}

std::vector<LevelZero::QueueInfo> LevelZero::queryQueueFamilies(ze_device_handle_t device) {
    // Get queue ordinals
    uint32_t numQueueGroups = 0;
    EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetCommandQueueGroupProperties(device, &numQueueGroups, nullptr));
    FATAL_ERROR_IF(numQueueGroups == 0, "No queue groups found!");
    std::vector<ze_command_queue_group_properties_t> queueProperties(numQueueGroups);
    EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetCommandQueueGroupProperties(device, &numQueueGroups, queueProperties.data()));

    // Iterate over queue groups
    std::vector<QueueInfo> result{};
    for (uint32_t i = 0; i < numQueueGroups; i++) {
        const bool isCompute = queueProperties[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE;
        const bool isCopy = queueProperties[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COPY;
        const bool isCopyOnly = !isCompute && isCopy;
        const size_t maxFillSize = queueProperties[i].maxMemoryFillPatternSize;
        const size_t count = queueProperties[i].numQueues;

        // Add compute and copy only queue
        if (isCopyOnly || isCompute) {
            ze_command_queue_desc_t desc = {};
            desc.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC;
            desc.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
            desc.ordinal = i;
            result.push_back({isCopyOnly, maxFillSize, count, device, desc});
        }
    }
    return result;
}

std::pair<ze_command_queue_handle_t, LevelZero::QueueInfo> LevelZero::createQueue(const QueueProperties &queueProperties) {
    if (!queueProperties.createQueue) {
        return {};
    }

    // Get device
    const ze_device_handle_t deviceForQueue = getDevice(queueProperties.deviceSelection);

    // Get queue info, which matches our requirements passed in QueueProperties
    const auto queueFamilies = queryQueueFamilies(deviceForQueue);
    const auto queueInfo = std::find_if(queueFamilies.begin(), queueFamilies.end(), [queueProperties](const QueueInfo &d) { return d.isCopyOnly == queueProperties.forceBlitter; });
    if (queueInfo == queueFamilies.end()) {
        FATAL_ERROR_IF(queueProperties.requireCreationSuccess, "Device does not support such queue");
        return {};
    }

    // Create
    const ze_command_queue_handle_t queue = createQueue(queueInfo->device, queueInfo->desc);
    return std::make_pair(queue, *queueInfo);
}

ze_command_queue_handle_t LevelZero::createQueue(ze_device_handle_t device, ze_command_queue_desc_t desc) {
    ze_command_queue_handle_t queue = {};
    EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueCreate(this->context, device, &desc, &queue));
    this->commandQueues.push_back(queue);
    return queue;
}

} // namespace L0
