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

#include "image_helper.h"

#include "framework/utility/error.h"

ImageType ImageHelper::getImageTypeFromDimensions(const size_t *dimensions) {
    if (dimensions[2] > 1) {
        return ImageType::Image3D;
    }
    if (dimensions[1] > 1) {
        return ImageType::Image2D;
    }
    return ImageType::Image1D;
}

size_t ImageHelper::getChannelCount(ChannelOrder order) {
    switch (order) {
    case ChannelOrder::R:
        return 1;
    case ChannelOrder::RG:
        return 2;
    case ChannelOrder::RGBA:
        return 4;
    default:
        FATAL_ERROR("Unknown channel order");
    }
}

size_t ImageHelper::getChannelFormatSize(ChannelFormat format) {
    switch (format) {
    case ChannelFormat::Float:
        return 4;
    default:
        FATAL_ERROR("Unknown channel format");
    }
}

size_t ImageHelper::getImageSizeInBytes(ChannelOrder order, ChannelFormat format, const size_t *dimensions) {
    return getChannelCount(order) * getChannelFormatSize(format) * dimensions[0] * dimensions[1] * dimensions[2];
}
