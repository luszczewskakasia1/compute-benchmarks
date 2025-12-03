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

Cuda::Cuda() : Cuda(Configuration::get().additionalConfiguration.cudaApiType) {}

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

void Cuda::launchKernel(void *function, dim3 gridSize, dim3 blockSize, void **args) {
    launchKernel(function, gridSize, blockSize, nullptr, args);
}

void Cuda::launchKernel(void *function, dim3 gridSize, dim3 blockSize, CUstream_st *stream, void **args) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaLaunchKernel(
            function,
            gridSize,
            blockSize,
            args,
            0,
            stream));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuLaunchKernel(
            reinterpret_cast<CUfunc_st *>(function),
            gridSize.x, gridSize.y, gridSize.z,
            blockSize.x, blockSize.y, blockSize.z,
            0,
            stream,
            args,
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

CUstream_st *Cuda::streamCreate() {
    return streamCreate(0);
}

CUstream_st *Cuda::streamCreate(unsigned int flags) {
    CUstream_st *stream;
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaStreamCreateWithFlags(&stream, flags));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuStreamCreate(&stream, flags));
    }
    return stream;
}

CUstream_st *Cuda::streamCreate(const StreamFlags &flags) {
    unsigned int flagsValue;
    if (apiType == CudaApiType::Runtime) {
        flagsValue = (flags.nonBlocking ? cudaStreamNonBlocking : 0);
    } else {
        flagsValue = (flags.nonBlocking ? CU_STREAM_NON_BLOCKING : 0);
    }

    return streamCreate(flagsValue);
}

void Cuda::streamDestroy(CUstream_st *stream) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaStreamDestroy(stream));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuStreamDestroy(stream));
    }
}

void Cuda::streamSynchronize(CUstream_st *stream) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaStreamSynchronize(stream));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuStreamSynchronize(stream));
    }
}

CUevent_st *Cuda::createEvent() {
    return createEvent(0);
}

CUevent_st *Cuda::createEvent(unsigned int flags) {
    CUevent_st *event;
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaEventCreateWithFlags(&event, flags));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuEventCreate(&event, flags));
    }
    return event;
}

CUevent_st *Cuda::createEvent(const EventFlags &flags) {
    unsigned int flagsValue;
    if (apiType == CudaApiType::Runtime) {
        flagsValue = (flags.blockingSync ? cudaEventBlockingSync : 0) | (flags.disableTiming ? cudaEventDisableTiming : 0) | (flags.interprocess ? cudaEventInterprocess : 0);
    } else {
        flagsValue = (flags.blockingSync ? CU_EVENT_BLOCKING_SYNC : 0) | (flags.disableTiming ? CU_EVENT_DISABLE_TIMING : 0) | (flags.interprocess ? CU_EVENT_INTERPROCESS : 0);
    }

    return createEvent(flagsValue);
}

void Cuda::eventDestroy(CUevent_st *event) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaEventDestroy(event));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuEventDestroy(event));
    }
}

void Cuda::eventRecord(CUevent_st *event, CUstream_st *stream) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaEventRecord(event, stream));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuEventRecord(event, stream));
    }
}

void *Cuda::memAlloc(size_t size) {
    if (apiType == CudaApiType::Runtime) {
        void *buffer = nullptr;
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaMalloc(&buffer, size));
        return buffer;
    } else {
        CUdeviceptr devicePtr;
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuMemAlloc(&devicePtr, size));
        return reinterpret_cast<void *>(devicePtr);
    }
}

void Cuda::memFree(void *ptr) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaFree(ptr));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuMemFree(reinterpret_cast<CUdeviceptr>(ptr)));
    }
}

void Cuda::memcpy(void *dst, const void *src, size_t size, MemcpyDirection direction) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaMemcpy(dst, src, size, static_cast<cudaMemcpyKind>(direction)));
    } else {
        switch (direction) {
        case MemcpyDirection::HostToDevice:
            EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuMemcpyHtoD(reinterpret_cast<CUdeviceptr>(dst), src, size));
            break;
        case MemcpyDirection::DeviceToHost:
            EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuMemcpyDtoH(dst, reinterpret_cast<CUdeviceptr>(const_cast<void *>(src)), size));
            break;
        case MemcpyDirection::DeviceToDevice:
            EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuMemcpyDtoD(reinterpret_cast<CUdeviceptr>(dst), reinterpret_cast<CUdeviceptr>(const_cast<void *>(src)), size));
            break;
        case MemcpyDirection::HostToHost:
        case MemcpyDirection::Default:
            EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuMemcpy(reinterpret_cast<CUdeviceptr>(dst), reinterpret_cast<CUdeviceptr>(const_cast<void *>(src)), size));
            break;
        }
    }
}
} // namespace CUDA
