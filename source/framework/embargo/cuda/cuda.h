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

#pragma once

#include "framework/additional/embargo/additional_configuration.h"
#include "framework/embargo/cuda/utility/error.h"
#include "framework/enum/embargo/cuda_api_embargo_type.h"
#include "framework/utility/error.h"

#include <cuda.h>
#include <cuda_runtime.h>
#include <framework/configuration.h>
#include <string>

namespace CUDA {

struct StreamFlags {
    bool nonBlocking = false;
};

struct EventFlags {
    bool disableTiming = false;
    bool blockingSync = false;
    bool interprocess = false;
};

enum class MemcpyDirection {
    HostToHost = cudaMemcpyHostToHost,
    HostToDevice = cudaMemcpyHostToDevice,
    DeviceToHost = cudaMemcpyDeviceToHost,
    DeviceToDevice = cudaMemcpyDeviceToDevice,
    Default = cudaMemcpyDefault
};

enum class StreamCaptureMode {
    Global = cudaStreamCaptureModeGlobal,
    ThreadLocal = cudaStreamCaptureModeThreadLocal,
    Relaxed = cudaStreamCaptureModeThreadLocal
};

class Cuda {
  public:
    Cuda();
    Cuda(CudaApiType type);
    ~Cuda();

    void synchronize() const;
    void *loadKernel(const std::string &fatbinFile, const std::string &kernelName);
    void launchKernel(void *function, dim3 gridSize, dim3 blockSize, void **args);
    void launchKernel(void *function, dim3 gridSize, dim3 blockSize, CUstream_st *stream, void **args);

    CUstream_st *streamCreate();
    CUstream_st *streamCreate(unsigned int flags);
    CUstream_st *streamCreate(const StreamFlags &flags);
    void streamDestroy(CUstream_st *stream);
    void streamSynchronize(CUstream_st *stream);

    CUevent_st *createEvent();
    CUevent_st *createEvent(unsigned int flags);
    CUevent_st *createEvent(const EventFlags &flags);
    void eventDestroy(CUevent_st *event);
    void eventRecord(CUevent_st *event, CUstream_st *stream);
    void eventRecordWithFlags(CUevent_st *event, CUstream_st *stream, bool recordExternal);
    void eventSynchronize(CUevent_st *event);
    uint64_t getEventDurationInNanoseconds(CUevent_st *start, CUevent_st *stop) const;

    CUgraph_st *graphCreate();
    void graphDestroy(CUgraph_st *graph);
    CUgraphExec_st *graphInstantiate(CUgraph_st *graph);
    void graphExecDestroy(CUgraphExec_st *graphExec);
    void graphLaunch(CUgraphExec_st *graphExec, CUstream_st *stream);
    CUgraphNode_st *graphAddKernelNode(CUgraph_st *graph, CUgraphNode_st **dependencies, size_t numDependencies, void *function, dim3 gridSize, dim3 blockSize, void **args);
    void graphAddDependencies(CUgraph_st *graph, CUgraphNode_st *from, CUgraphNode_st *to, const CUgraphEdgeData_st *edgeData, const size_t numEdges);

    void streamBeginCapture(CUstream_st *stream);
    void streamBeginCapture(CUstream_st *stream, enum StreamCaptureMode mode);
    CUgraph_st *streamEndCapture(CUstream_st *stream);

    void *memAlloc(size_t size);
    void memFree(void *ptr);

    void memcpy(void *dst, const void *src, size_t size, MemcpyDirection direction);

  private:
    CudaApiType apiType;
    cudaLibrary_t library = nullptr;

    // Runtime API state
    int deviceId = 0;

    // Driver API state
    CUdevice device = 0;
    CUcontext context = nullptr;
};

} // namespace CUDA

using namespace CUDA;
