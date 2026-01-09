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
    eventRecordWithFlags(event, stream, false);
}

void Cuda::eventRecordWithFlags(CUevent_st *event, CUstream_st *stream, bool recordExternal) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaEventRecordWithFlags(event, stream, recordExternal ? cudaEventRecordExternal : cudaEventRecordDefault));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuEventRecordWithFlags(event, stream, recordExternal ? CU_EVENT_RECORD_EXTERNAL : CU_EVENT_RECORD_DEFAULT));
    }
}

void Cuda::eventSynchronize(CUevent_st *event) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaEventSynchronize(event));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuEventSynchronize(event));
    }
}

uint64_t Cuda::getEventDurationInNanoseconds(CUevent_st *start, CUevent_st *stop) const {
    float timeMs;
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaEventElapsedTime(&timeMs, start, stop));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuEventElapsedTime(&timeMs, start, stop));
    }
    return static_cast<uint64_t>(timeMs * 1e6);
}

CUgraph_st *Cuda::graphCreate() {
    CUgraph_st *graph;
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGraphCreate(&graph, 0));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuGraphCreate(&graph, 0));
    }
    return graph;
}

void Cuda::graphDestroy(CUgraph_st *graph) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGraphDestroy(graph));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuGraphDestroy(graph));
    }
}

CUgraphExec_st *Cuda::graphInstantiate(CUgraph_st *graph) {
    CUgraphExec_st *graphExec;
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGraphInstantiate(&graphExec, graph, 0));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuGraphInstantiate(&graphExec, graph, 0));
    }
    return graphExec;
}

void Cuda::graphExecDestroy(CUgraphExec_st *graphExec) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGraphExecDestroy(graphExec));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuGraphExecDestroy(graphExec));
    }
}

void Cuda::graphLaunch(CUgraphExec_st *graphExec, CUstream_st *stream) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGraphLaunch(graphExec, stream));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuGraphLaunch(graphExec, stream));
    }
}

CUgraphNode_st *Cuda::graphAddKernelNode(CUgraph_st *graph, CUgraphNode_st **dependencies, size_t numDependencies,
                                         void *function, dim3 gridSize, dim3 blockSize, void **args) {
    CUgraphNode_st *node;
    if (apiType == CudaApiType::Runtime) {
        cudaKernelNodeParams nodeParams = {};
        nodeParams.func = function;
        nodeParams.gridDim = gridSize;
        nodeParams.blockDim = blockSize;
        nodeParams.kernelParams = args;
        nodeParams.sharedMemBytes = 0;
        nodeParams.extra = nullptr;
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGraphAddKernelNode(&node, graph, dependencies, numDependencies, &nodeParams));
    } else {
        CUDA_KERNEL_NODE_PARAMS nodeParams = {};
        nodeParams.func = reinterpret_cast<CUfunction>(function);
        nodeParams.gridDimX = gridSize.x;
        nodeParams.gridDimY = gridSize.y;
        nodeParams.gridDimZ = gridSize.z;
        nodeParams.blockDimX = blockSize.x;
        nodeParams.blockDimY = blockSize.y;
        nodeParams.blockDimZ = blockSize.z;
        nodeParams.kernelParams = args;
        nodeParams.sharedMemBytes = 0;
        nodeParams.extra = nullptr;
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuGraphAddKernelNode(&node, graph, dependencies, numDependencies, &nodeParams));
    }
    return node;
}

void Cuda::graphAddDependencies(CUgraph_st *graph, CUgraphNode_st *from, CUgraphNode_st *to, const CUgraphEdgeData_st *edgeData, const size_t numEdges) {
    static_assert(offsetof(cudaGraphEdgeData, from_port) == offsetof(CUgraphEdgeData, from_port));
    static_assert(offsetof(cudaGraphEdgeData, to_port) == offsetof(CUgraphEdgeData, to_port));
    static_assert(offsetof(cudaGraphEdgeData, type) == offsetof(CUgraphEdgeData, type));
    static_assert(offsetof(cudaGraphEdgeData, reserved) == offsetof(CUgraphEdgeData, reserved));
    static_assert(sizeof(cudaGraphEdgeData) == sizeof(CUgraphEdgeData));

    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGraphAddDependencies(graph, &from, &to, reinterpret_cast<const cudaGraphEdgeData *>(edgeData), numEdges));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuGraphAddDependencies(graph, &from, &to, edgeData, numEdges));
    }
}

void Cuda::streamBeginCapture(CUstream_st *stream) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaStreamBeginCapture(stream, cudaStreamCaptureModeGlobal));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuStreamBeginCapture(stream, CU_STREAM_CAPTURE_MODE_GLOBAL));
    }
}

void Cuda::streamBeginCapture(CUstream_st *stream, enum StreamCaptureMode mode) {
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaStreamBeginCapture(stream, static_cast<cudaStreamCaptureMode>(mode)));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuStreamBeginCapture(stream, static_cast<CUstreamCaptureMode>(mode)));
    }
}

CUgraph_st *Cuda::streamEndCapture(CUstream_st *stream) {
    CUgraph_st *graph;
    if (apiType == CudaApiType::Runtime) {
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaStreamEndCapture(stream, &graph));
    } else {
        EXPECT_CUDA_DRIVER_RESULT_SUCCESS(cuStreamEndCapture(stream, &graph));
    }
    return graph;
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
