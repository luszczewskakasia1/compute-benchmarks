#pragma once

#include "framework/enum/buffer_contents.h"
#include "framework/ocl/cl.h"
#include "framework/ocl/error.h"
#include "framework/ocl/function_signatures_ocl.h"
#include "framework/utility/buffer_contents_helper.h"

#include <memory>

class BufferContentsHelperOcl : public BufferContentsHelper {
  public:
    static cl_int fillBuffer(cl_command_queue queue, cl_mem buffer, size_t bufferSize, BufferContents contents) {
        switch (contents) {
        case BufferContents::Zeros:
            return fillBufferWithZeros(queue, buffer, bufferSize);
        case BufferContents::Random:
            return fillBufferWithRandomBytes(queue, buffer, bufferSize);
        default:
            ERROR("Unknown buffer contents");
        }
    }

    static cl_int fillUsmBuffer(cl_command_queue queue, void *usmBuffer, size_t bufferSize, BufferContents contents) {
        switch (contents) {
        case BufferContents::Zeros:
            return fillUsmBufferWithZeros(queue, usmBuffer, bufferSize);
        case BufferContents::Random:
            return fillUsmBufferWithRandomBytes(queue, usmBuffer, bufferSize);
        default:
            ERROR("Unknown buffer contents");
        }
    }

  private:
    static cl_int fillBufferWithRandomBytes(cl_command_queue queue, cl_mem buffer, size_t bufferSize) {
        auto cpuBuffer = std::make_unique<uint8_t[]>(bufferSize);
        BufferContentsHelper::fillWithRandomBytes(cpuBuffer.get(), bufferSize);
        CL_SUCCESS_OR_RETURN(clEnqueueWriteBuffer(queue, buffer, CL_BLOCKING, 0, bufferSize, cpuBuffer.get(), 0, nullptr, nullptr));
        return CL_SUCCESS;
    }

    static cl_int fillBufferWithZeros(cl_command_queue queue, cl_mem buffer, size_t bufferSize) {
        const cl_uint pattern[] = {0};
        CL_SUCCESS_OR_RETURN(clEnqueueFillBuffer(queue, buffer, pattern, sizeof(pattern), 0, bufferSize, 0, nullptr, nullptr));
        CL_SUCCESS_OR_RETURN(clFinish(queue));
        return CL_SUCCESS;
    }

    static cl_int fillUsmBufferWithRandomBytes(cl_command_queue queue, void *usmBuffer, size_t bufferSize) {
        // Get API calls
        cl_device_id device = {};
        CL_SUCCESS_OR_RETURN(clGetCommandQueueInfo(queue, CL_QUEUE_DEVICE, sizeof(device), &device, nullptr));
        cl_context context = {};
        CL_SUCCESS_OR_RETURN(clGetCommandQueueInfo(queue, CL_QUEUE_CONTEXT, sizeof(context), &context, nullptr));
        cl_platform_id platform = {};
        CL_SUCCESS_OR_RETURN(clGetDeviceInfo(device, CL_DEVICE_PLATFORM, sizeof(platform), &platform, nullptr));
        auto clHostMemAllocINTEL = (pfn_clHostMemAllocINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clHostMemAllocINTEL");
        auto clMemFreeINTEL = (pfn_clMemFreeINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clMemFreeINTEL");
        auto clEnqueueMemcpyINTEL = (pfn_clEnqueueMemcpyINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueMemcpyINTEL");

        // Create staging allocation
        cl_int retVal = {};
        void *stagingAlloc = clHostMemAllocINTEL(context, nullptr, bufferSize, 0, &retVal);
        CL_SUCCESS_OR_RETURN(retVal);
        fillWithRandomBytes(static_cast<uint8_t *>(stagingAlloc), bufferSize);

        // Copy to destination allocation
        CL_SUCCESS_OR_RETURN(clEnqueueMemcpyINTEL(queue, CL_NON_BLOCKING, usmBuffer, stagingAlloc, bufferSize, 0, nullptr, nullptr));
        CL_SUCCESS_OR_RETURN(clFinish(queue));
        CL_SUCCESS_OR_RETURN(clMemFreeINTEL(context, stagingAlloc));

        return CL_SUCCESS;
    }

    static cl_int fillUsmBufferWithZeros(cl_command_queue queue, void *usmBuffer, size_t bufferSize) {
        // Get API calls
        cl_device_id device = {};
        CL_SUCCESS_OR_RETURN(clGetCommandQueueInfo(queue, CL_QUEUE_DEVICE, sizeof(device), &device, nullptr));
        cl_platform_id platform = {};
        CL_SUCCESS_OR_RETURN(clGetDeviceInfo(device, CL_DEVICE_PLATFORM, sizeof(platform), &platform, nullptr));
        auto clEnqueueMemFillINTEL = (pfn_clEnqueueMemFillINTEL)clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueMemFillINTEL");

        // Fill with zeros
        const cl_uint pattern[] = {0};
        CL_SUCCESS_OR_RETURN(clEnqueueMemFillINTEL(queue, usmBuffer, pattern, sizeof(pattern), bufferSize, 0, nullptr, nullptr));
        CL_SUCCESS_OR_RETURN(clFinish(queue));

        return CL_SUCCESS;
    }
};
