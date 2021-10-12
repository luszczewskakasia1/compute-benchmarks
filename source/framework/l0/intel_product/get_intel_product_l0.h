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

#include "framework/intel_product/get_intel_product.h"
#include "framework/l0/levelzero.h"

IntelProduct getIntelProduct(ze_device_handle_t device);
IntelProduct getIntelProduct(const ze_device_properties_t &deviceProperties);
IntelProduct getIntelProduct(const LevelZero &levelzero);
