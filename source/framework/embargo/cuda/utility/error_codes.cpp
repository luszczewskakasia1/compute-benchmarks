/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2025-2026 Intel Corporation
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

#include "error_codes.h"

const char *cudaErrorToString(cudaError_t retVal) {
    switch (retVal) {
    case cudaSuccess:
        return "cudaSuccess";
    case cudaErrorInvalidValue:
        return "cudaErrorInvalidValue";
    case cudaErrorMemoryAllocation:
        return "cudaErrorMemoryAllocation";
    case cudaErrorInitializationError:
        return "cudaErrorInitializationError";
    case cudaErrorCudartUnloading:
        return "cudaErrorCudartUnloading";
    case cudaErrorProfilerDisabled:
        return "cudaErrorProfilerDisabled";
    case cudaErrorInvalidConfiguration:
        return "cudaErrorInvalidConfiguration";
    case cudaErrorInvalidSymbol:
        return "cudaErrorInvalidSymbol";
    case cudaErrorInvalidDevicePointer:
        return "cudaErrorInvalidDevicePointer";
    case cudaErrorInvalidMemcpyDirection:
        return "cudaErrorInvalidMemcpyDirection";
    case cudaErrorInsufficientDriver:
        return "cudaErrorInsufficientDriver";
    case cudaErrorNoDevice:
        return "cudaErrorNoDevice";
    case cudaErrorInvalidDevice:
        return "cudaErrorInvalidDevice";
    case cudaErrorLaunchFailure:
        return "cudaErrorLaunchFailure";
    case cudaErrorLaunchTimeout:
        return "cudaErrorLaunchTimeout";
    case cudaErrorLaunchOutOfResources:
        return "cudaErrorLaunchOutOfResources";
    case cudaErrorIllegalAddress:
        return "cudaErrorIllegalAddress";
    default:
        return "Unknown CUDA error";
    }
}

const char *cudaErrorToString(CUresult retVal) {
    switch (retVal) {
    case CUDA_SUCCESS:
        return "CUDA_SUCCESS";
    case CUDA_ERROR_NOT_INITIALIZED:
        return "CUDA_ERROR_NOT_INITIALIZED";
    case CUDA_ERROR_INVALID_VALUE:
        return "CUDA_ERROR_INVALID_VALUE";
    case CUDA_ERROR_INVALID_DEVICE:
        return "CUDA_ERROR_INVALID_DEVICE";
    case CUDA_ERROR_INVALID_CONTEXT:
        return "CUDA_ERROR_INVALID_CONTEXT";
    case CUDA_ERROR_INVALID_HANDLE:
        return "CUDA_ERROR_INVALID_HANDLE";
    case CUDA_ERROR_OUT_OF_MEMORY:
        return "CUDA_ERROR_OUT_OF_MEMORY";
    case CUDA_ERROR_NOT_FOUND:
        return "CUDA_ERROR_NOT_FOUND";
    case CUDA_ERROR_LAUNCH_FAILED:
        return "CUDA_ERROR_LAUNCH_FAILED";
    case CUDA_ERROR_FILE_NOT_FOUND:
        return "CUDA_ERROR_FILE_NOT_FOUND";
    case CUDA_ERROR_INVALID_IMAGE:
        return "CUDA_ERROR_INVALID_IMAGE";
    case CUDA_ERROR_NO_BINARY_FOR_GPU:
        return "CUDA_ERROR_NO_BINARY_FOR_GPU";
    case CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES:
        return "CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES";
    case CUDA_ERROR_LAUNCH_TIMEOUT:
        return "CUDA_ERROR_LAUNCH_TIMEOUT";
    case CUDA_ERROR_ILLEGAL_ADDRESS:
        return "CUDA_ERROR_ILLEGAL_ADDRESS";
    default:
        return "Unknown CUDA error";
    }
}
