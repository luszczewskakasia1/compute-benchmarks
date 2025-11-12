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

#pragma once

#include "framework/embargo/cuda/utility/error.h"
#include "framework/enum/embargo/cuda_api_embargo_type.h"
#include "framework/utility/error.h"

#include <cuda.h>
#include <cuda_runtime.h>
#include <string>

namespace CUDA {

class Cuda {
  public:
    Cuda(CudaApiType type = CudaApiType::Runtime);
    ~Cuda();

    void synchronize() const;
    void *loadKernel(const std::string &fatbinFile, const std::string &kernelName);
    void launchKernel(void *function, dim3 gridSize, dim3 blockSize);

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
