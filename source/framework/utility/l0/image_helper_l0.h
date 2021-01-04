#pragma once

#include "framework/utility/error.h"
#include "framework/utility/image_helper.h"

#include <level_zero/ze_api.h>

struct ImageHelperL0 : ImageHelper {
    static inline ze_image_type_t getL0ImageTypeFromDimensions(const size_t *dimensions) {
        switch (ImageHelper::getImageTypeFromDimensions(dimensions)) {
        case ImageType::Image1D:
            return ZE_IMAGE_TYPE_1D;
        case ImageType::Image2D:
            return ZE_IMAGE_TYPE_2D;
        case ImageType::Image3D:
            return ZE_IMAGE_TYPE_3D;
        case ImageType::Image1D_Array:
            return ZE_IMAGE_TYPE_1DARRAY;
        case ImageType::Image2D_Array:
            return ZE_IMAGE_TYPE_2DARRAY;
        case ImageType::Image_Buffer:
            return ZE_IMAGE_TYPE_BUFFER;
        default:
            ERROR("Unknown image type");
        }
    }

    static inline ze_image_format_t getImageFormat(ChannelOrder order, ChannelFormat format) {
        ze_image_format_t imageFormat = {};
        switch (order) {
        case ChannelOrder::R:
            imageFormat.layout = ZE_IMAGE_FORMAT_LAYOUT_32;
            imageFormat.x = ZE_IMAGE_FORMAT_SWIZZLE_R;
            imageFormat.y = ZE_IMAGE_FORMAT_SWIZZLE_0;
            imageFormat.z = ZE_IMAGE_FORMAT_SWIZZLE_0;
            imageFormat.w = ZE_IMAGE_FORMAT_SWIZZLE_1;
            break;
        case ChannelOrder::RG:
            imageFormat.layout = ZE_IMAGE_FORMAT_LAYOUT_32_32;
            imageFormat.x = ZE_IMAGE_FORMAT_SWIZZLE_R;
            imageFormat.y = ZE_IMAGE_FORMAT_SWIZZLE_G;
            imageFormat.z = ZE_IMAGE_FORMAT_SWIZZLE_0;
            imageFormat.w = ZE_IMAGE_FORMAT_SWIZZLE_1;
            break;
        case ChannelOrder::RGBA:
            imageFormat.layout = ZE_IMAGE_FORMAT_LAYOUT_32_32_32_32;
            imageFormat.x = ZE_IMAGE_FORMAT_SWIZZLE_R;
            imageFormat.y = ZE_IMAGE_FORMAT_SWIZZLE_G;
            imageFormat.z = ZE_IMAGE_FORMAT_SWIZZLE_B;
            imageFormat.w = ZE_IMAGE_FORMAT_SWIZZLE_A;
            break;
        default:
            ERROR("Unknown channel order");
        }

        switch (format) {
        case ChannelFormat::Float:
            imageFormat.type = ZE_IMAGE_FORMAT_TYPE_FLOAT;
            break;
        default:
            ERROR("Unknown channel format");
        }

        return imageFormat;
    }
};
