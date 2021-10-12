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

#include "framework/enum/image_type.h"

#include <cstddef>

struct ImageHelper {
    enum class ChannelOrder {
        Unknown,
        R,
        RG,
        RGBA,
    };

    enum class ChannelFormat {
        Unknown,
        Float,
    };

    static ImageType getImageTypeFromDimensions(const size_t *dimensions);
    static size_t getChannelCount(ChannelOrder order);
    static size_t getChannelFormatSize(ChannelFormat format);
    static size_t getImageSizeInBytes(ChannelOrder order, ChannelFormat format, const size_t *dimensions);
};
