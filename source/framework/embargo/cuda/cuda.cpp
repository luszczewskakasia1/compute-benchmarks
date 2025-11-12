/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2025 Intel Corporation
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

#include "cuda.h"

#include <fstream>
#include <vector>

namespace CUDA {

Cuda::Cuda(CudaApiType type) : apiType(type) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaSetDevice(deviceId));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuInit(0));
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuDeviceGet(&device, 0));

        CUctxCreateParams params = {};
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuCtxCreate(&context, &params, 0, device));
    }
}

Cuda::~Cuda() {
    if (library) {
        cudaLibraryUnload(library);
        library = nullptr;
    }
    if (apiType == CudaApiType::Runtime) {
        cudaDeviceReset();
    } else {
        if (context) {
            cuCtxDestroy(context);
            context = nullptr;
        }
    }
}

void *Cuda::loadKernel(const std::string &fatbinFileName, const std::string &kernelName) {
    std::string foundPath;
    const std::vector<std::string> searchPaths = {
        fatbinFileName,
        "./" + fatbinFileName,
        "../" + fatbinFileName,
        "../../" + fatbinFileName,
    };

    for (const auto &path : searchPaths) {
        std::ifstream file(path, std::ios::binary);
        if (file.good()) {
            foundPath = path;
            break;
        }
    }
    if (foundPath.empty()) {
        FATAL_ERROR("Could not find fatbin file: " + fatbinFileName);
    }

    cudaKernel_t kernel;
    EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaLibraryLoadFromFile(&library, foundPath.c_str(), nullptr, nullptr, 0, nullptr, nullptr, 0));
    EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaLibraryGetKernel(&kernel, library, kernelName.c_str()));
    return reinterpret_cast<void *>(kernel);
}

void Cuda::launchKernel(void *function, dim3 gridSize, dim3 blockSize) {
    if (apiType == CudaApiType::Runtime) {
        cudaKernel_t kernel = reinterpret_cast<cudaKernel_t>(function);
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaLaunchKernel(
            reinterpret_cast<const void *>(kernel),
            gridSize,
            blockSize,
            nullptr,
            0,
            nullptr));
    } else {
        CUfunction cuFunction = reinterpret_cast<CUfunction>(function);
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuLaunchKernel(
            cuFunction,
            gridSize.x, gridSize.y, gridSize.z,
            blockSize.x, blockSize.y, blockSize.z,
            0,
            nullptr,
            nullptr,
            nullptr));
    }
}

void Cuda::synchronize() const {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaDeviceSynchronize());
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuCtxSynchronize());
    }
}

} // namespace CUDA
