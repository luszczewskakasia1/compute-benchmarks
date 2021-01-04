#pragma once

#include "framework/ocl/cl.h"
#include "framework/utility/error.h"
#include "framework/utility/image_helper.h"

struct ImageHelperOcl : ImageHelper {
    static inline cl_mem_object_type getOclImageTypeFromDimensions(const size_t *dimensions) {
        switch (ImageHelper::getImageTypeFromDimensions(dimensions)) {
        case ImageType::Image1D:
            return CL_MEM_OBJECT_IMAGE1D;
        case ImageType::Image2D:
            return CL_MEM_OBJECT_IMAGE2D;
        case ImageType::Image3D:
            return CL_MEM_OBJECT_IMAGE3D;
        case ImageType::Image1D_Array:
            return CL_MEM_OBJECT_IMAGE1D_ARRAY;
        case ImageType::Image2D_Array:
            return CL_MEM_OBJECT_IMAGE2D_ARRAY;
        case ImageType::Image_Buffer:
            return CL_MEM_OBJECT_IMAGE1D_BUFFER;
        default:
            ERROR("Unknown image type");
        }
    }

    static inline cl_image_format getImageFormat(ChannelOrder order, ChannelFormat format) {
        cl_image_format imageFormat = {};

        switch (order) {
        case ChannelOrder::R:
            imageFormat.image_channel_order = CL_R;
            break;
        case ChannelOrder::RG:
            imageFormat.image_channel_order = CL_RG;
            break;
        case ChannelOrder::RGBA:
            imageFormat.image_channel_order = CL_RGBA;
            break;
        default:
            ERROR("Unknown channel order");
        }

        switch (format) {
        case ChannelFormat::Float:
            imageFormat.image_channel_data_type = CL_FLOAT;
            break;
        default:
            ERROR("Unknown channel format");
        }

        return imageFormat;
    }
};
