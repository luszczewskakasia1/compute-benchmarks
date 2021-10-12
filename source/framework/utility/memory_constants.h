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

namespace MemoryConstants {
constexpr inline size_t cachelineSize = 64;
constexpr inline size_t kiloByte = 1024u;
constexpr inline size_t megaByte = 1024u * kiloByte;
constexpr inline size_t gigaByte = 1024u * megaByte;
} // namespace MemoryConstants
