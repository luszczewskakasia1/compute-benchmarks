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

#include <level_zero/ze_api.h>
#include <level_zero/zex_api.h>

namespace L0 {
using L0ImportExternalPointer = decltype(&zexDriverImportExternalPointer);
using L0ReleaseImportedPointer = decltype(&zexDriverReleaseImportedPointer);
using L0GetHostPointerBaseAddress = decltype(&zexDriverGetHostPointerBaseAddress);

struct ExtensionProperties {
    bool getImportHostPointerFunctions = false;

    static ExtensionProperties create() {
        return ExtensionProperties();
    }

    ExtensionProperties &setImportHostPointerFunctions(bool value) {
        getImportHostPointerFunctions = value;
        return *this;
    }
};
} // namespace L0
