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

#include "framework/l0/levelzero.h"
#include "framework/utility/hex_helper.h"

#include "utility/multi_process_helper.h"

#include <unordered_map>

struct MultiProcessHelperL0 : MultiProcessHelper {
    static ze_result_t getSubDevicesForExecution(LevelZero &levelzero,
                                                 DeviceSelection subDevices,
                                                 size_t processesPerDevice,
                                                 std::vector<DeviceSelection> &outSubDevicesForExecution) {
        const uint32_t actualSubDevicesCount = static_cast<uint32_t>(levelzero.getSubDevicesCount());
        outSubDevicesForExecution = MultiProcessHelper::getSubDevicesForExecution(subDevices, processesPerDevice, actualSubDevicesCount);
        return ZE_RESULT_SUCCESS;
    }

    struct BufferForSubDevice {
        BufferForSubDevice() = default;
        BufferForSubDevice(const BufferForSubDevice &) = delete;
        BufferForSubDevice &operator=(const BufferForSubDevice &) = delete;
        BufferForSubDevice(BufferForSubDevice &&) = default;
        BufferForSubDevice &operator=(BufferForSubDevice &&) = default;

        void *buffer = {};
        size_t sizeForOneProcess = {};
        size_t totalSize = {};
        int fileDescriptor;

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
            bufferForSubDevice.totalSize = bufferForSubDevice.sizeForOneProcess * processesPerSubDevice;

            const ze_device_mem_alloc_desc_t deviceAllocationDesc{ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC};
            ZE_RESULT_SUCCESS_OR_RETURN(zeMemAllocDevice(levelzero.context, &deviceAllocationDesc, bufferForSubDevice.totalSize, 0, levelzero.getDevice(subDevice), &bufferForSubDevice.buffer));

            ze_ipc_mem_handle_t ipcHandle = {};
            ZE_RESULT_SUCCESS_OR_RETURN(zeMemGetIpcHandle(levelzero.context, bufferForSubDevice.buffer, &ipcHandle));
            bufferForSubDevice.fileDescriptor = *reinterpret_cast<int *>(ipcHandle.data);

            outBuffersForSubDevices[subDevice] = std::move(bufferForSubDevice);
        }

        return ZE_RESULT_SUCCESS;
    }
};
