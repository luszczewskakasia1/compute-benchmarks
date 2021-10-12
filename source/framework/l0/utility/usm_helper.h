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

#include "framework/enum/device_selection.h"
#include "framework/enum/usm_memory_placement.h"
#include "framework/l0/levelzero.h"

namespace L0::UsmHelper {

ze_result_t allocate(UsmMemoryPlacement placement, LevelZero &levelZero, size_t size, void **buffer);

ze_result_t deallocate(UsmMemoryPlacement placement, LevelZero &levelZero, void *buffer);

ze_result_t allocate(DeviceSelection placement, LevelZero &levelzero, size_t size, void **outBuffer);

} // namespace L0::UsmHelper
