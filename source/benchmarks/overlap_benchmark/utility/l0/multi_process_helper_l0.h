#pragma once

#include "framework/l0/levelzero.h"
#include "framework/utility/hex_helper.h"

#include "utility/multi_process_helper.h"

#include <unordered_map>

struct MultiProcessHelperL0 : MultiProcessHelper {
    static ze_result_t getSubDevicesForExecution(DeviceSelection subDevices,
                                                 size_t processesPerDevice,
                                                 ze_device_handle_t rootDevice,
                                                 std::vector<DeviceSelection> &outSubDevicesForExecution) {
        uint32_t actualSubDevicesCount = {};
        ZE_RESULT_SUCCESS_OR_RETURN(zeDeviceGetSubDevices(rootDevice, &actualSubDevicesCount, nullptr));
        outSubDevicesForExecution = MultiProcessHelper::getSubDevicesForExecution(subDevices, processesPerDevice, actualSubDevicesCount);
        return ZE_RESULT_SUCCESS;
    }

    struct BufferForSubDevice {
        void *buffer = {};
        size_t sizeForOneProcess = {};
        size_t totalSize = {};
        std::string ipcHandle = {};

        size_t getNextOffset() {
            return sizeForOneProcess * (currentProcessesCount++);
        }

      private:
        size_t currentProcessesCount = {};
    };

    static ze_result_t allocateSharedBuffersForSubDevices(LevelZero &levelzero,
                                                          DeviceSelection subDevices,
                                                          size_t processesPerSubDevice,
                                                          size_t workgroupsPerProcess,
                                                          size_t threadsPerWorkgroup,
                                                          std::unordered_map<DeviceSelection, BufferForSubDevice> &outBuffersForSubDevices) {
        for (DeviceSelection subDevice : DeviceSelectionHelper::split(subDevices)) {
            BufferForSubDevice bufferForSubDevice = {};
            bufferForSubDevice.sizeForOneProcess = workgroupsPerProcess * threadsPerWorkgroup * sizeof(uint32_t);
            bufferForSubDevice.totalSize = bufferForSubDevice.sizeForOneProcess * threadsPerWorkgroup;

            const ze_device_mem_alloc_desc_t deviceAllocationDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
            ZE_RESULT_SUCCESS_OR_RETURN(zeMemAllocDevice(levelzero.context, &deviceAllocationDesc, bufferForSubDevice.totalSize, 0, levelzero.getDevice(subDevice), &bufferForSubDevice.buffer));

            ze_ipc_mem_handle_t ipcHandle = {};
            ZE_RESULT_SUCCESS_OR_RETURN(zeMemGetIpcHandle(levelzero.context, bufferForSubDevice.buffer, &ipcHandle));
            bufferForSubDevice.ipcHandle = HexHelper::toHex(reinterpret_cast<unsigned char *>(ipcHandle.data), ZE_MAX_IPC_HANDLE_SIZE);

            outBuffersForSubDevices[subDevice] = std::move(bufferForSubDevice);
        }

        return ZE_RESULT_SUCCESS;
    }
};
