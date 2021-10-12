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

#include "framework/enum/engine.h"
#include "framework/l0/utility/error.h"

#include <level_zero/ze_api.h>
#include <memory>
#include <vector>

namespace L0 {
class QueueFamiliesHelper {
  public:
    struct QueueFamilyDesc {
        ze_device_handle_t device;
        uint32_t ordinal;
        size_t maxFillSize;
        size_t queueCount;
        EngineGroup type;
    };

    struct QueueDesc {
        QueueFamilyDesc family;
        ze_command_queue_desc_t desc;
        ze_command_queue_handle_t queue;
    };

    static std::unique_ptr<QueueDesc> getPropertiesForSelectingEngine(ze_device_handle_t device, Engine engine);

  private:
    static std::vector<QueueFamilyDesc> queryQueueFamilies(ze_device_handle_t device);
    static EngineGroup getEngineGroup(ze_command_queue_group_properties_t family);
};
} // namespace L0
