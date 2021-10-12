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

#include "framework/l0/utility/print_device_info_l0.inl"
#include "framework/print_device_info.h"
#include "framework/supported_apis.h"
#include "framework/utility/execute_at_app_init.h"

EXECUTE_AT_APP_INIT {
    DeviceInfo::registerFunctions(Api::L0, L0::printDeviceInfo, L0::printAvailableDevices);
    SupportedApis::registerSupportedApi(Api::L0);
};
