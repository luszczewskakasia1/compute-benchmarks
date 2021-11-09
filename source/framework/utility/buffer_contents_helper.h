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

#include "framework/enum/buffer_contents.h"

#include <cstring>
#include <random>

class BufferContentsHelper {
  public:
    static void fill(uint8_t *buffer, size_t size, BufferContents contents);
    static void fillWithZeros(uint8_t *buffer, size_t size);
    static void fillWithRandomBytes(uint8_t *buffer, size_t size);
    static void fillWithIncreasingBytes(uint8_t *buffer, size_t size);

  private:
    static thread_local std::mt19937 generator;
    static std::vector<uint8_t> cachedRandomData;

    static uint64_t randomOword();
};
